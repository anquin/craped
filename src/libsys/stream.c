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
#include <stdio.h>


static FileError configFileIO_(Stream *stream)
{
  LLFile *llfile;
  FileError error;
  llfile = fileOpen_ll(stream->filepath, stream->openMode, &error);
  if (error == FILE_ERROR_NO_ERROR) {
    FILEIO(llfile)->mode = FILEIO(llfile)->mode;
    FILEIO(llfile)->status = FILEIO(llfile)->status;
    fileClose(FILEIO(llfile));
  }
  return error;
}

FileError readToBuffer_(Stream *stream, Position *where)
{
  /* TODO: there must be a better to do this. */
  Byte *buf;
  Size nbytes;
  LLFile *llfile;
  FileError error;
  llfile = fileOpen_ll(stream->filepath, stream->openMode, &error);
  if (error != FILE_ERROR_NO_ERROR) {
    return error;
  }
  buf = (Byte *)malloc(sizeof(Byte) * maxPageSize);
  do {
    nbytes = fileRead(FILEIO(llfile), buf, maxPageSize);
    pagedRawDataInsert(stream->data, *where, buf, nbytes);
    (*where) += nbytes;
  } while (nbytes);
  free(buf);
  return fileClose(FILEIO(llfile));
}

Stream *openStream_(char *filepath, FileOpenMode mode, PagedRawData *data,
                    short ownsData, FileError *error)
{
  FileIO *fileio;
  Stream *stream;
  Position where;

  *error = FILE_ERROR_NO_ERROR;

  fileio = (FileIO *)malloc(sizeof(FileIO));
  stream = (Stream *)malloc(sizeof(Stream));
  fileio->impl.stream = stream;
  stream->file = fileio;
  fileio->type = 1;

  STREAM(fileio)->ownsData = ownsData;
  STREAM(fileio)->data = data;

  validateFileOpenMode(&mode);

  stream->filepath = filepath;
  stream->openMode = mode;
  configFileIO_(stream);

  if ((mode & FILE_OPEN_MODE_R) && (!(mode & FILE_OPEN_MODE_TRUNC))) {
    where = 0;
    *error = readToBuffer_(STREAM(fileio), &where);
    if (*error != FILE_ERROR_NO_ERROR) {
      if (*error == FILE_ERROR_NOT_EXIST
          && mode & FILE_OPEN_MODE_CREATE) {
        /* if (!(createEmptyFile(filepath))) { */
        /*   return(openStream_(filepath, mode, data, ownsData, error)); */
        /* } */
        *error = FILE_ERROR_NO_ERROR;
      } else {
        free(stream);free(fileio);
        fileio = NULL;
        stream = NULL;
      }
    }
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
  if ((STREAM(fileio)->ownsData) && (STREAM(fileio)->data != NULL)) {
    pagedRawDataDestroy(STREAM(fileio)->data);
  }
  return FILE_ERROR_NO_ERROR;
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

FileError fileCommit_stream(FileIO *fileio)
{
  Byte *bytes;
  Size size;
  FileError ferr;
  FileIO *dest_file;

  if (fileMode(fileio) == FILE_MODE_RO) {
    /* TODO: What does it mean to commit a non writeable stream? */
    return FILE_ERROR_NO_ERROR;
  }

  size = pagedRawDataSize(STREAM(fileio)->data);
  bytes = malloc(sizeof(Byte) * size);
  pagedRawDataRead(STREAM(fileio)->data, 0, bytes, size);
  dest_file = FILEIO(fileOpen_ll(STREAM(fileio)->filepath,
                                 STREAM(fileio)->openMode, &ferr));
  fileWrite(dest_file, bytes, size);
  fileClose(dest_file);
  free(bytes);
  free(dest_file);
  return FILE_ERROR_NO_ERROR;

  /* TODO: */
  /* This is unefficient, better aproach would be to create a function, */
  /* for example rawDataFetchChunks(), that would return an array of direct */
  /* pointers to the stored data portions, then iterate through this array */
  /* and call fileWrite() so it becomes unecessary to mallocate an entire new */
  /* intermediate buffer containing the whole file content in order to
     write it. */
}
