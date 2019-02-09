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

#include "libsys/raw_data.h"
#include "raw_data_private.h"
#include "libsys/def.h"
#include <libsys/quicksearch.h>

#include <stdlib.h>

Position
rawDataSearchForward(RawData *data, Byte *pattern, Size size, Position start)
{
  Position pos;
  static Size qsBc[ALPHABET_SIZE];
  static Byte *lastPattern = NULL;
  Size tmpSz;

  /* Performs a simple pointer comparison hoping to avoid startup */
  if (pattern != lastPattern) {
    preQsBcForward(pattern, size, qsBc);
  }

  if (start < data->gapByte) {
    pos = quickSearchForward(pattern, size,
			     data->begin + start, data->gapByte, qsBc);

    tmpSz = data->gapByte - size;

    if (pos < tmpSz) {
      return pos;
    }

    /* gap byte not in last data's position */
    if (data->gapByte >= tmpSz) {
      rawDataMoveGap_(data, data->gapByte - tmpSz);
      start = 0;
    }

  }

  pos = quickSearchForward(pattern, size,
			   data->begin + data->gapByte + data->gapSize + start,
			   data->size - data->gapByte, qsBc);

  return pos + data->gapByte;
}

Position
rawDataSearchBackward(RawData *data, Byte *pattern, Size size, Position start)
{
  return data->size;
}
