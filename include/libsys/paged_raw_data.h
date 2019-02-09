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

#ifndef PAGED_RAW_DATA_H
#define PAGED_RAW_DATA_H

#include "paging.h"

typedef struct pagedRawData
{
  Size size;
  PageTable pageTable;
} PagedRawData;

#define createPagedRawData() (pagedRawDataInit((PagedRawData *)malloc(sizeof(PagedRawData))))
PagedRawData *pagedRawDataInit(PagedRawData *buf);
void pagedRawDataDestroy(PagedRawData *buf);

/* Returns the size of the pagedRawData (number of bytes written in it). */
#define pagedRawDataSize(buf) ((buf)->size)

/* Inserts the specified amount of bytes from the given position onwards.
   Returns the amount of bytes that were written. */
Size pagedRawDataInsert(PagedRawData *buf, Position where, Byte *src, Size size);

/* Deletes the specified amount of bytes from the given position onwards.
   Returns the amount of bytes that were deleted. */
Size pagedRawDataDelete(PagedRawData *buf, Position where, Size size);

/* Copies the specified amount of bytes from a given position onwards
   to dest. Returns the amount of bytes actually read. */
Size pagedRawDataRead(PagedRawData *buf, Position where, Byte *dest, Size size);

/* Returns data size if pattern is not found */
Position
pagedRawDataSearchForward(PagedRawData *buf, Byte *pattern, Size size, Position start);
/* Returns data size if pattern is not found */
Position
pagedRawDataSearchBackward(PagedRawData *buf, Byte *pattern, Size size, Position start);

#endif
