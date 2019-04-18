/*
 * Copyright (C) 2016 Andre Q. Barbosa
 *
 * This file is part of Craped.
 *
 * Craped is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libsys/paged_raw_data.h>
#include <libsys/filesystem.h>
#include <libsys/stream.h>
#include "buffer.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_MARK_HASH_MOD 151

void initBuffer(Buffer *buf, char *name)
{
  strncpy(buf->name, name, BUFFER_NAME_MAX);
  buf->name[BUFFER_NAME_MAX - 1] = '\0';
  pagedRawDataInit(&buf->data);
  buf->flags = 0;
  buf->bytePoint = 0;
  strcpy(buf->filePath, "\0");
  initMarkChain(&buf->markChain, BUFFER_MARK_HASH_MOD);
}

void destroyBuffer(Buffer *buf)
{
  pagedRawDataDestroy(&buf->data);
}

char *bufferGetName(Buffer *buf)
{
  char *name;
  unsigned len;

  len = strlen(buf->name) + 1;
  name = (char *)malloc(sizeof(char) * len);

  strncpy(name, buf->name, len);

  return name;
}

UInt32 bufferGetFlags(Buffer *buf)
{
  return buf->flags;
}

void bufferSetFlags(Buffer *buf, UInt32 flags)
{
  buf->flags |= flags;
}

void bufferResetFlags(Buffer *buf, UInt32 flags)
{
  buf->flags &= (~flags);
}

/* NOTE:
 * Editing functions proxy to PagedRawData somehow
*/

Size bufferGetChunk(Buffer *buf, Byte *dest, Size size)
{
  return pagedRawDataRead(&buf->data, buf->bytePoint, dest, size);
}

Size bufferGetChunkByMarkName(Buffer *buf, Byte *dest, char *markName)
{
  Mark *match;
  Position markPos;

  match = markChainFind(&buf->markChain, markName);
  if (match == NULL) return 0;

  markPos = markGetPosition(match);
  if (buf->bytePoint < markPos) {
    return pagedRawDataRead(&buf->data, buf->bytePoint,
                            dest, markPos - buf->bytePoint);
  }
  return pagedRawDataRead(&buf->data, markPos, dest, buf->bytePoint - markPos);
}

Size bufferInsert(Buffer *buf, Byte *src, Size size)
{
  if (buf->flags & BUFFER_FLAG_RDONLY) return 0;

  size = pagedRawDataInsert(&buf->data, buf->bytePoint, src, size);
  buf->bytePoint += size;
  buf->flags |= BUFFER_FLAG_MODIFIED;

  bufferResetFlags(buf, BUFFER_FLAG_EOB);
  bufferResetFlags(buf, BUFFER_FLAG_BOB);
  return size;
}

Size bufferDelete(Buffer *buf, Size size)
{
  if (buf->flags & BUFFER_FLAG_RDONLY) return 0;

  assert(bufferSize(buf) >= buf->bytePoint + size);

  buf->flags |= BUFFER_FLAG_MODIFIED;
  return pagedRawDataDelete(&buf->data, buf->bytePoint, size);
}

Position bufferGetPoint(Buffer *buf)
{
  return buf->bytePoint;
}

void bufferSetPoint(Buffer *buf, Position position)
{
  assert((position >= 0) && (position <= bufferSize(buf)));
  buf->bytePoint = position;

  if (position != bufferSize(buf)) {
    bufferResetFlags(buf, BUFFER_FLAG_EOB);
  }
  if (position) {
    bufferResetFlags(buf, BUFFER_FLAG_BOB);
  }
}

void bufferMovePointForward(Buffer *buf, Size size)
{
  Size bufSz = bufferSize(buf);
  bufferResetFlags(buf, BUFFER_FLAG_BOB);
  if (size > (bufSz - buf->bytePoint)) {
    buf->bytePoint = bufSz;
    bufferSetFlags(buf, BUFFER_FLAG_EOB);
  }
  else if (size) {
    buf->bytePoint += size;
    bufferResetFlags(buf, BUFFER_FLAG_EOB);
  }
}

void bufferMovePointBackward(Buffer *buf, Size size)
{
  bufferResetFlags(buf, BUFFER_FLAG_EOB);
  if (size > buf->bytePoint) {
    buf->bytePoint = 0;
    bufferSetFlags(buf, BUFFER_FLAG_BOB);
  }
  else if (size) {
    buf->bytePoint -= size;
    bufferResetFlags(buf, BUFFER_FLAG_BOB);
  }
}

Size bufferSize(Buffer *buf)
{
  return pagedRawDataSize(&buf->data);
}

Position bufferSearch(Buffer *buf, Byte *pattern, Size patternSize)
{
  abort();
}

char *bufferGetFilePath(Buffer *buf)
{
  return buf->filePath;
}

void bufferSetFilePath(Buffer *buf, char *filePath)
{
  strncpy(buf->filePath, filePath, FILE_PATH_MAX);
}

void bufferSetPersistenceFilePath(Buffer *buf, char *filePath)
{
  unsigned len = strlen(filePath);
  assert(len < FILE_PATH_MAX);
  strncpy(buf->filePath, filePath, len);
}

void bufferRead(Buffer *buf)
{
  Byte *file_content;
  Size max;
  Stream *file_stream;
  FileError file_error;

  max = fileLength(buf->filePath);
  assert(max != 0);
  file_stream = openStreamWithExistingBuffer(buf->filePath, FILE_OPEN_MODE_R,
                                             &buf->data, &file_error);
  assert(file_error == FILE_ERROR_NO_ERROR);

  file_content = (Byte *)malloc(sizeof(Byte) * max);
  fileRead(FILEIO(file_stream), file_content, max);
  /* pagedRawDataClearToEnd(&buf->data, buf->bytePoint); */
  buf->flags &= (!BUFFER_FLAG_MODIFIED);

  fileClose(FILEIO(file_stream));
}

void bufferWrite(Buffer *buf)
{
  unsigned max = 512;
  Stream *file_stream;
  FileError file_error;

  file_stream = openStreamWithExistingBuffer(buf->filePath, FILE_OPEN_MODE_W,
                                             &buf->data, &file_error);
  assert(file_error == FILE_ERROR_NO_ERROR);

  if (fileCommit(FILEIO(file_stream)) == FILE_ERROR_NO_ERROR) {
    buf->flags &= (!BUFFER_FLAG_MODIFIED);
  }

  fileClose(FILEIO(file_stream));
}

void bufferAddMark(Buffer *buf, char *name, Byte flags)
{
  markChainPushFront(&buf->markChain, name, buf->bytePoint, flags);
}

void bufferRemoveMark(Buffer *buf, char *name)
{
  markChainRemove(&buf->markChain, name);
}

void bufferMarkToPoint(Buffer *buf, char *markName)
{
  Mark *match;

  match = markChainFind(&buf->markChain, markName);
  if (match != NULL) {
    buf->bytePoint = markGetPosition(match);
  }
}

void bufferPointToMark(Buffer *buf, char *markName)
{
  Mark *match;

  match = markChainFind(&buf->markChain, markName);
  if (match != NULL) {
    markSetPosition(match, buf->bytePoint);
  }
}

void bufferSetMark(Buffer *buf, char *markName, Position bytePos)
{
  Mark *match;

  assert(bytePos < bufferSize(buf));

  match = markChainFind(&buf->markChain, markName);
  if (match != NULL) {
    match->bytePos = bytePos;
  }
}

short bufferIsPointAtMark(Buffer *buf, char *markName)
{
  Mark *match;

  match = markChainFind(&buf->markChain, markName);

  return (match->bytePos == buf->bytePoint);
}
