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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libsys/raw_data.h"

#define GAP_POS(data) (data->gapByte)
#define GAP_SIZE(data) (data->gapSize)
#define DATA_SIZE(data) (data->size)
#define DATA_BEGIN(data) (data->begin)
#define DATA_PTR(data, pos) (data->begin + pos)

RawData *rawDataInit(RawData *data, Size size)
{
  DATA_SIZE(data) = 0;
  DATA_BEGIN(data) = (Byte *)malloc(sizeof(Byte) * size);
  GAP_POS(data) = 0;
  GAP_SIZE(data) = size;

  return data;
}

void rawDataDestroy(RawData *data)
{
  free(DATA_BEGIN(data));
}

void rawDataMoveGap_(RawData *data, Position where)
{
  Size cpySz;
  void *cpyFrom, *cpyTo;

  if (where == GAP_POS(data)) {
    return;
  }

  if (GAP_SIZE(data)) {
    if (where < GAP_POS(data)) {
      cpySz = GAP_POS(data) - where;
      cpyFrom = DATA_PTR(data, where);
      cpyTo = cpyFrom + GAP_SIZE(data);
    }

    else {
      cpySz = where - GAP_POS(data);
      cpyTo = DATA_PTR(data, GAP_POS(data));
      cpyFrom = cpyTo + GAP_SIZE(data);
    }

    memmove(cpyTo, cpyFrom, cpySz);
  }
  GAP_POS(data) = where;
}

Size rawDataInsert(RawData *data, Position where, Byte *src, Size size)
{
  assert(where <= rawDataSize(data));

  if (GAP_SIZE(data) < size) {
    assert(GAP_SIZE(data) >= size);
    return 0;
  }

  if (GAP_POS(data) != where) {
    rawDataMoveGap_(data, where);
  }

  DATA_SIZE(data) += size;
  memcpy(DATA_PTR(data, GAP_POS(data)), src, size);
  GAP_POS(data) += size;
  GAP_SIZE(data) -= size;

  return size;
}

Size rawDataDelete(RawData *data, Position where, Size size)
{
  assert(where < (rawDataSize(data)));
  if (where >= (rawDataSize(data))) {
    return 0;
  }

  if (GAP_POS(data) != where) {
    rawDataMoveGap_(data, where);
  }

  DATA_SIZE(data) -= size;
  GAP_SIZE(data) += size;

  return size;
}

Size rawDataRead(RawData *data, Position where, Byte *dest, Size size)
{
  assert(dest != NULL);

  if (where < GAP_POS(data)) {
    if ((where + size) <= GAP_POS(data)) {
      memcpy(dest, DATA_PTR(data, where), size);
    }
    else {
      Size bread;
      bread = GAP_POS(data) - where;
      memcpy(dest, DATA_PTR(data, where), bread);
      memcpy(dest + bread, DATA_PTR(data, GAP_POS(data) + GAP_SIZE(data)), size - bread);
    }
  }
  else {
    memcpy(dest, DATA_PTR(data, where + GAP_SIZE(data)), size);
  }

  return size;
}
