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

#ifndef BUFFER_INTERNAL_H
#define BUFFER_INTERNAL_H

#include "subeditor/def.h"
#include <libsys/paged_raw_data.h>
#include "time.h"
#include "mark.h"

#define BUFFER_FLAG_MODIFIED 1 << 0
#define BUFFER_FLAG_BOB 1 << 1
#define BUFFER_FLAG_EOB 1 << 2
#define BUFFER_FLAG_RDONLY 1 << 3
#define BUFFER_FLAG_READ_ERROR 1 << 4

typedef struct buffer
{
  /* Buffer name seen by the user */
  char name[BUFFER_NAME_MAX];

  /* Contents of the buffer */
  PagedRawData data;

  /* Last time at which the buffer contents were identical to its source */
  Time dataTime;

  /* Stores buffer flags. bits 0 to 15 are defined by the buffer,
     16 to 31 are free to general purposes */
  UInt32 flags;

  /* Stores the current position */
  Position bytePoint;

  /* Name of the buffer's persistence file, if any, blank string otherwise */
  char filePath[FILE_PATH_MAX];

  MarkChain markChain;

} Buffer;


/* Initializes the buffer */
void initBuffer(Buffer *buf, char *name);

void destroyBuffer(Buffer *buf);

char *bufferGetName(Buffer *buf);

UInt32 bufferGetFlags(Buffer *buf);
void bufferSetFlags(Buffer *buf, UInt32 flags);
void bufferResetFlags(Buffer *buf, UInt32 flags);

/* NOTE:
 * Editing functions proxies to RawData somehow
*/

/* Writes in 'dest' a copy of the chunk from the buffer current position
   Returns the total of bytes fetched */
Size bufferGetChunk(Buffer *buf, Byte *dest, Size size);

/* Writes in 'dest' a copy of the chunk from the buffer 
   between the current position and the mark position
   Returns the total of bytes fetched */
Size bufferGetChunkByMarkName(Buffer *buf, Byte *dest, char *markName);

/* Writes to the buffer current position */
Size bufferInsert(Buffer *buf, Byte *src, Size size);

/* Delete a number of bytes beggining in the current position */
Size bufferDelete(Buffer *buf, Size size);

Position bufferGetPoint(Buffer *buf);

void bufferSetPoint(Buffer *buf, Position position);

void bufferMovePointForward(Buffer *buf, Size size);
void bufferMovePointBackward(Buffer *buf, Size size);

Size bufferSize(Buffer *buf);

/* Searches for the first occurrence of a pattern from the cursor position */
Position bufferSearch(Buffer *buf, Byte *pattern, Size patternSize);

char *bufferGetFilePath(Buffer *buf);
void bufferSetFilePath(Buffer *buf, char *filePath);

/* I/O to file */
void bufferRead(Buffer *buf);
void bufferWrite(Buffer *buf);

/* == Mark Procedures == */
/* Creates a mark at the current point */
void bufferAddMark(Buffer *buf, char *name, Byte flags);
void bufferRemoveMark(Buffer *buf, char *mark);
void bufferPointToMark(Buffer *buf, char *markName);
void bufferMarkToPoint(Buffer *buf, char *markName);
void bufferSetMark(Buffer *buf, char *markName, Position bytePos);
short bufferIsPointAtMark(Buffer *buf, char *markName);

/* Returns buffer's size if pattern is not found */
Position bufferSearchForward(Buffer *buf, Byte *pattern);
Position bufferSearchBackward(Buffer *buf, Byte *pattern);

#endif
