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

#include <libsys/llfile.h>
#include <libsys/dbgdie.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

LLFile *fileOpen_ll(char *filepath, FileOpenMode mode, FileError *error)
{
  FileIO *fileio;
  int opnflags;
  int opnmode;
  LLFile *llfile;

  *error = validateFileOpenMode(&mode);
  if (*error != FILE_ERROR_NO_ERROR) {
    return NULL;
  }

  fileio = (FileIO *)malloc(sizeof(FileIO));
  llfile = (LLFile *)malloc(sizeof(LLFile));
  fileio->impl.llfile = llfile;
  llfile->file = fileio;
  fileio->type = 0;

  opnmode = 0;
  opnflags = 0;
  if ((mode & FILE_OPEN_MODE_R) && (mode & FILE_OPEN_MODE_W)) {
    opnflags |= O_RDWR;
    opnflags |= O_APPEND;
  } else {
    if (mode & FILE_OPEN_MODE_R) {
      opnflags |= O_RDONLY;
    }
    if (mode & FILE_OPEN_MODE_W) {
      opnflags |= O_WRONLY;
      opnflags |= O_APPEND;
    }
  }
  if (mode & FILE_OPEN_MODE_CREATE) {
    opnflags |= O_CREAT;
    opnmode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
  }
  if (mode & FILE_OPEN_MODE_APPEND) {
    opnflags |= O_APPEND;
  }
  if (mode & FILE_OPEN_MODE_TRUNC) {
    opnflags |= O_TRUNC;
  }

  llfile->fd = open(filepath, opnflags, opnmode);
  if (llfile->fd == -1) {
    *error = FILE_ERROR_CANNOT_OPEN;
    free(llfile);free(fileio);
    fileio = NULL;
    llfile = NULL;
  }
  else {
    *error = FILE_ERROR_NO_ERROR;
  }
  return llfile;
}

FileError fileClose_ll(FileIO *fileio)
{
  if (close(LLFILE(fileio)->fd) == -1) {
    return FILE_ERROR_ON_CLOSE;
  }
  return FILE_ERROR_NO_ERROR;
}


void fileSeek_ll(FileIO *fileio, int offset, FileIOWhence whence)
{
  Position pos;
  int c_whence;
  if (whence == FILE_IO_WHENCE_BEG) c_whence = SEEK_SET;
  else if (whence == FILE_IO_WHENCE_CUR) c_whence = SEEK_CUR;
  else if (whence == FILE_IO_WHENCE_END) c_whence = SEEK_END;
  pos = lseek(LLFILE(fileio)->fd, offset, whence);
  if (pos == -1) {
    die("lseek() failed.\n");
  }
  LLFILE(fileio)->pos = pos;
}

Position fileTell_ll(FileIO *fileio)
{
  return LLFILE(fileio)->pos;
}

Size fileRead_ll(FileIO *fileio, Byte *content, Size sz)
{
  Size readSz;
  readSz = read(LLFILE(fileio)->fd, content, sz);
  if (readSz == -1) {
    die("read() failed.\n");
  }
  return readSz;
}

Size fileWrite_ll(FileIO *fileio, Byte *content, Size sz)
{
  Size writeSz;
  writeSz = write(LLFILE(fileio)->fd, content, sz);
  if (writeSz == -1) {
    die("write() failed\n");
  }
  return writeSz;
}


FileError fileCommit_ll(FileIO *fileio)
{
  /* Does nothing */
  return FILE_ERROR_NO_ERROR;
}
