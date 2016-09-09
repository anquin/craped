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

#include <libsys/stream.h>
#include <libsys/llfile.h>
#include <libsys/dbglog.h>
#include "page_factory.h"

#include <stdlib.h>

void readToBuffer_(Stream *stream, Position *where)
{
  /* TODO: there must be a better to do this. */
  Byte *buf;
  Size nbytes;
  buf = (Byte *)malloc(sizeof(Byte) * maxPageSize);
  do {
    nbytes = fileRead_ll(FILEIO(stream->llfile), buf, maxPageSize);
    pagedRawDataInsert(stream->data, *where, buf, nbytes);
    (*where) += nbytes;
  } while (nbytes);
}

Stream *openStream_(char *filepath, FileOpenMode mode, PagedRawData *data,
                    short ownsData, FileError *error)
{
  FileIO *fileio;
  Stream *stream;
  Byte *buffer;
  Position where;

  fileio = (FileIO *)malloc(sizeof(FileIO));
  stream = (Stream *)malloc(sizeof(Stream));
  fileio->impl.stream = stream;
  stream->file = fileio;
  fileio->type = 1;

  STREAM(fileio)->ownsData = ownsData;
  STREAM(fileio)->data = data;

  buffer = (Byte *)malloc(sizeof(maxPageSize));

  STREAM(fileio)->llfile = fileOpen_ll(filepath, mode, error);
  if (*error != FILE_ERROR_NO_ERROR) {
    free(stream);free(fileio);
    fileio = NULL;
    stream = NULL;
  }
  else {
    where = 0;
    readToBuffer_(STREAM(fileio), &where);
  }
  return stream;
}

Stream *
openStreamWithExistingBuffer(char *filepath, FileOpenMode mode,
                             PagedRawData *data, FileError *error)
{
  return openStream_(filepath, mode, data, 0, error);
}

Stream *
fileOpen_stream(char *filepath, FileOpenMode mode, FileError *error)
{
  PagedRawData *data = createPagedRawData();
  return openStream_(filepath, mode, data, 1, error);
}

FileError fileClose_stream(FileIO *fileio)
{
  FileError error;
  error = fileClose_ll(FILEIO(STREAM(fileio)->llfile));
  if ((STREAM(fileio)->ownsData) && (STREAM(fileio)->data != NULL)) {
    pagedRawDataDestroy(STREAM(fileio)->data);
  }
  return error;
}


void fileSeek_stream(FileIO *fileio, int loc, FileIOWhence whence)
{
  switch (whence) {
  case FILE_IO_WHENCE_BEG:
    STREAM(fileio)->loc = loc;
    break;
  case FILE_IO_WHENCE_CUR:
    STREAM(fileio)->loc = STREAM(fileio)->loc + loc;
    break;
  case FILE_IO_WHENCE_END:
    STREAM(fileio)->loc = rawDataSize(STREAM(fileio)->data) - loc;
    break;
  }
}

Position fileTell_stream(FileIO *fileio)
{
  return STREAM(fileio)->loc;
}

Size fileRead_stream(FileIO *fileio, Byte *content, Size sz)
{
  return
    pagedRawDataRead(STREAM(fileio)->data, STREAM(fileio)->loc, content, sz);
}

Size fileWrite_stream(FileIO *fileio, Byte *content, Size sz)
{
  return
    pagedRawDataInsert(STREAM(fileio)->data, STREAM(fileio)->loc, content, sz);
}

void fileCommit_stream(FileIO *fileio)
{
  /* TODO */
  DBGLOG("fileCommit_stream() not implemented yet.\n", 0);
}
