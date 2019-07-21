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

#ifndef LIBSYS_FILEIO_H
#define LIBSYS_FILEIO_H

#include "def.h"

typedef unsigned FileOpenMode;
extern const FileOpenMode FILE_OPEN_MODE_R;
extern const FileOpenMode FILE_OPEN_MODE_W;
extern const FileOpenMode FILE_OPEN_MODE_RW;
extern const FileOpenMode FILE_OPEN_MODE_APPEND;
extern const FileOpenMode FILE_OPEN_MODE_CREATE;
extern const FileOpenMode FILE_OPEN_MODE_TRUNC;

typedef enum file_error
{
  FILE_ERROR_NO_ERROR=0,
  FILE_ERROR_CANNOT_CREATE,
  FILE_ERROR_CANNOT_OPEN,
  FILE_ERROR_NOT_EXIST,
  FILE_ERROR_NO_PERMISSION,
  FILE_ERROR_INVALID_OPEN_MODE,
  FILE_ERROR_ON_CLOSE,
  FILE_ERROR_OPERATION_NOT_IMPLEMENTED
} FileError;

typedef enum file_mode
{
  FILE_MODE_RO=1,
  FILE_MODE_WO,
  FILE_MODE_RW
} FileMode;

typedef enum file_io_status
{
  FILE_IO_STATUS_OK,
  FILE_IO_STATUS_EOF,
  FILE_IO_STATUS_CLOSED
} FileIOStatus;

typedef enum file_io_whence
{
  FILE_IO_WHENCE_BEG,
  FILE_IO_WHENCE_CUR,
  FILE_IO_WHENCE_END
} FileIOWhence;

typedef struct ll_file LLFile;
typedef struct stream Stream;
typedef struct ram_file RAMFile;

typedef struct file_io
{
  int type;
  union
  {
    LLFile *llfile;
    Stream *stream;
    /* RAMFIle *ramfile; */
  } impl;
  FileMode mode;
  FileIOStatus status;

} FileIO;

#define FILEIO(o) (o ? o->file : (void *)0)
#define LLFILE(o) (o ? o->impl.llfile : (void *)0)
#define STREAM(o) (o ? o->impl.stream : (void *)0)
#define RAMFILE(o) (o ? o->impl.ramfile : (void *)0)

#ifndef FILE_DEFUN  /* Not been defined in fileio.c */
#define FILE_DEFUN(ret_type, func_id, argdecl, arglst) \
  ret_type func_id ## _ll argdecl;                     \
  ret_type func_id ## _stream argdecl;                 \
  ret_type func_id ## _ramFile argdecl;                \
  ret_type func_id argdecl;
#endif

/* Returns zero if 0k */
FILE_DEFUN(FileError, fileClose, (FileIO *fileio), (fileio));

FILE_DEFUN(void, fileSeek, (FileIO *fileio, int offset, FileIOWhence whence), (fileio, offset, whence));
FILE_DEFUN(Position, fileTell, (FileIO *fileio), (fileio));

FILE_DEFUN(Size, fileRead, (FileIO *fileio, Byte *content, Size sz), (fileio, content, sz));
FILE_DEFUN(Size, fileWrite, (FileIO *fileio, Byte *content, Size sz), (fileio, content, sz));

/* Should do nothing if not stream */
FILE_DEFUN(FileError, fileCommit, (FileIO *fileio), (fileio));

#define fileMode(fileio) (((FileIO *)fileio)->mode)

/* This function has side effects: fileOpenMode will be changed
   whenever a mode can be inferred. Eg.: write without read or
   append means also truncate. */
FileError validateFileOpenMode(FileOpenMode *mode);

#endif
