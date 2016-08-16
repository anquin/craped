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

#include "stream.h"
#include "page_factory.h"

#include <stdlib.h>

void readToBuffer_(Position *where, PagedRawData *buf)
{
  /* TODO: there must be a better to do this. */
  Size nbytes;
  do {
    nbytes = fileRead_ll(FILEOBJ(STREAM)->llfile, buffer, maxPageSize);
    pagedRawDataInsert(STREAM(fileio)->data, *where, buffer, nbytes);
    (*where) += nbytes;
  } while (nbytes);
}

FileIO *openStream_(char *filepath, FileOpenMode mode, PagedRawData *data,
                    PagedRawData *selfData, FileError *error)
{
  FileIO *fileio;
  Stream *stream;
  Byte *buffer;
  Position where;

  fileio = (FileIO *)malloc(sizeof(FileIO));
  stream = (Stream *)malloc(sizeof(Stream));
  fileio->impl.stream = stream;

  STREAM(fileio->selfData) = selfData;
  STREAM(fileio->data) = data;

  buffer = (Byte *)malloc(sizeof(maxPageSize));

  STREAM(fileio)->llfile = fileOpen_ll(filename, mode, error);
  if (error != FILE_ERROR_NO_ERROR) {
    free(stream);free(fileio);
    fileio = stream = NULL;
  }
  else {
    where = 0;
    readToBuffer_(&where, STREAM(fileio)->data);
  }
  return fileio;
}

Stream *
openStreamWithExistingBuffer(char *filepath, FileOpenMode mode,
                             PagedRawData *data, FileError *error)
{
  return openStream_(filePath, mode, data, data, error);
}

FileIO *
fileOpen_stream(FileIO *fileio, char *filepath,
                FileOpenMode mode, FileError *error)
{
  return openStream_(filePath, mode, data, NULL, error);
}

FileError fileClose_stream(FileIO *fileio)
{
  FileError error;
  error = fileClose_ll(FILEOBJ(STREAM(fileio)->llfile));
  if (STREAM(fileio)->selfData != NULL) {
    pagedRawDataDestroy(STREAM(fileio)->selfData);
  }
  return error;
}


void fileSeek_stream(FileIO *fileio, Position loc, FileIOWhence whence)
{
  switch (whence) {
  case FILE_IO_WHENCE_BEG:
    STREAM(fileio)->loc = loc;
    break;
  case FILE_IO_WHENCE_CUR:
    STREAM(fileio)->loc = STREAM(fileio)->loc + loc;
    break;
  case FILE_IO_WHENCE_END:
    STREAM(fileio)->loc = rawDataSize(selfData) - loc;
    break;
  }
}

Position fileTell_stream(FileIO *fileio)
{
  return STREAM(fileio)->loc;
}

Size fileRead_stream(FileIO *fileio, Byte *content, Size sz)
{
  Size nbytes =
    pagedRawDataRead(STREAM(fileio)->data, STREAM(fileio)->loc, content, sz);
  if (!nbytes) {
    /* The source may have new data. */
    readToBuffer_(&(STREAM(fileio)->loc), STREAM(fileio)->data);
  }
  return nbytes;
}

Size fileWrite_stream(FileIO *fileio, Byte *content, Size sz)
{
  return
    pagedRawDataInsert(STREAM(fileio)->data, STREAM(fileio)->loc, content, sz);
}

void fileCommit_stream(FileIO *fileio)
{
  /* TODO */
  DBGLOG("fileCommit_stream() not implemented yet.\n");
}
