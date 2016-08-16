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
 * Editing functions proxies to RawData somehow
*/

Size bufferGetChunk(Buffer *buf, Byte *dest, Size size)
{
  return pagedRawDataRead(&buf->data, buf->bytePoint, dest, size);
}

Size bufferInsert(Buffer *buf, Byte *src, Size size)
{
  buf->flags |= BUFFER_FLAG_MODIFIED;
  size = pagedRawDataInsert(&buf->data, buf->bytePoint, src, size);
  buf->bytePoint += size;

  return size;
}

Size bufferDelete(Buffer *buf, Size size)
{
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
}

void bufferMovePoint(Buffer *buf, int offset)
{
  Position position = buf->bytePoint + offset;

  assert((position >= 0) && (position <= bufferSize(buf)));
  buf->bytePoint = position;
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
/* FIXME: RawData should do this and this function should proxy it */
{
  unsigned i;
  FILE *f;
  Byte *src;
  unsigned max = 512;

  f = fopen(buf->filePath, "rb");
  assert(f);

  src = (Byte *)malloc(sizeof(Byte) * max);

  for (i = 0; !feof(f); i += max) {
    fread(src, sizeof(Byte), max, f);
    pagedRawDataInsert(&buf->data, i, src, max);
  }
  /* pagedRawDataClearToEnd(&buf->data, buf->bytePoint); */

  buf->flags ^= BUFFER_FLAG_MODIFIED;
  fclose(f);
}

void bufferWrite(Buffer *buf)
/* FIXME: RawData should do this and this function should proxy it */
{
  unsigned i, bread;
  FILE *f;
  Byte *src;
  unsigned max = 512;

  f = fopen(buf->filePath, "wb");
  assert(f);

  src = (Byte *)malloc(sizeof(Byte) * max);

  i = 0;
  while (bread = pagedRawDataRead(&buf->data, i, src, max)) {
    fwrite(src, sizeof(Byte), bread, f);
    i += bread;
  }

  buf->flags ^= BUFFER_FLAG_MODIFIED;
  free(src);
  fclose(f);
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
