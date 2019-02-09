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

#ifndef LIBSYS_DBG_LOG_H
#define LIBSYS_DBG_LOG_H

#ifdef DEBUG_MODE
#include <stdio.h>
#include <string.h>
FILE *dbgLogFile;
#define DBGLOG_OPEN(fd) dbgLogFile = fd
#define DBGLOG(s, ...) if (dbgLogFile) {  \
    fprintf(dbgLogFile, s, __VA_ARGS__);          \
    fflush(dbgLogFile);                           \
  }
#define DBGLOG_CLOSE() if (dbgLogFile) fclose(dbgLogFile);

#else
#define DBGLOG_OPEN(fd)
#define DBGLOG(s, ...)
#define DBGLOG_CLOSE()

#endif

#endif
