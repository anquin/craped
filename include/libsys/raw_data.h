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

#ifndef RAW_DATA_H
#define RAW_DATA_H

#include "def.h"

typedef struct raw_data
{
  Size size;
  Byte *begin;
  Position gapByte;
  Size gapSize;
} RawData;

#define newRawData(size) (rawDataInit((RawData *)malloc(sizeof(RawData)), size))
RawData *rawDataInit(RawData *data, Size size);
void rawDataDestroy(RawData *data);

#define rawDataSize(data) ((data)->size)
Size rawDataInsert(RawData *data, Position where, Byte *src, Size size);
Size rawDataDelete(RawData *data, Position where, Size size);
Size rawDataRead(RawData *data, Position where, Byte *dest, Size size);
#define rawDataCapacity(data) ((data->size) + (data)->gapSize)

/* Returns data size if pattern is not found */
Position
rawDataSearchForward(RawData *data, Byte *pattern, Size size, Position start);
/* Returns data size if pattern is not found */
Position
rawDataSearchBackward(RawData *data, Byte *pattern, Size size, Position start);

#endif
