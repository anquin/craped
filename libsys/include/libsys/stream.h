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

#ifndef LIBSYS_STREAM_H
#define LIBSYS_STREAM_H

#include "fileio.h"
#include "paged_raw_data.h"

struct stream
{
  FileIO *file;

  /* The buffer used by the stream. Equals to
     selfData in case the stream owns it. */
  PagedRawData *data;

  /* The low level io channel */
  /* LLFile *llfile; */

  Position loc;

  char *filepath;

  /* Non-zero if the stream owns the buffer it is using */
  short ownsData;
};

Stream *
fileOpen_stream(char *filepath, FileOpenMode mode, FileError *error);

Stream *
openStreamWithExistingBuffer(char *filepath, FileOpenMode mode,
                             PagedRawData *data, FileError *error);

#endif
