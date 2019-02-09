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
#include <assert.h>

#include "libsys/raw_data.h"
#include "libsys/paging.h"
#include "libsys/paged_raw_data.h"


PagedRawData *pagedRawDataInit(PagedRawData *buf)
{
  pageTableInit(&(buf->pageTable));
  buf->size = rawDataSize(buf->pageTable.firstPage->data);
  return buf;
}

void pagedRawDataDestroy(PagedRawData *buf)
{
  pageTableDestroy(&(buf->pageTable));
}

Size pagedRawDataInsert(PagedRawData *buf, Position where, Byte *src, Size size)
{
  Size currPgSpcLeft, insertSize, rawDataSizeBefore;
  PageHint *pageHint;

  if (where > pagedRawDataSize(buf)) {
    /* We are helpless. */
    return 0;
  }

  rawDataSizeBefore = buf->size;
  pageHint = pageTableHint(&(buf->pageTable));

  do {
    assert (pageTableFindPage(&(buf->pageTable), where) != PAGE_NOT_FOUND);

    currPgSpcLeft = rawDataCapacity(pageHint->pagePtr->data)
      - rawDataSize(pageHint->pagePtr->data);
    if (!currPgSpcLeft) {
      pageTableSplitPage(&(buf->pageTable));
      continue;
    }
    insertSize = ((size < currPgSpcLeft) ? size : currPgSpcLeft);

    rawDataInsert(pageHint->pagePtr->data, pageHint->offset, src, insertSize);
    buf->size += insertSize;
    where += insertSize;
    size -= insertSize;
    src += insertSize;
  } while (size);

  return buf->size - rawDataSizeBefore;
}

Size pagedRawDataDelete(PagedRawData *buf, Position where, Size size)
{
  Size currPgSpcAfterPos, deleteSize, rawDataSizeBefore;
  PageHint *pageHint;

  rawDataSizeBefore = buf->size;
  pageHint = pageTableHint(&(buf->pageTable));

  do {
    if (pageTableFindPage(&(buf->pageTable), where) == PAGE_NOT_FOUND) {
      abort();
    }
    currPgSpcAfterPos =
      rawDataSize(pageHint->pagePtr->data) - (where - pageHint->pageBegin);
    deleteSize = ((size < currPgSpcAfterPos) ? size : currPgSpcAfterPos);

    if (deleteSize) {
      if (deleteSize == rawDataSize(pageHint->pagePtr->data)) {
	pageTableDeletePage(&(buf->pageTable));
      }
      else {
	rawDataDelete(pageHint->pagePtr->data, pageHint->offset, deleteSize);
      }
      buf->size -= deleteSize;
      size -= deleteSize;
    }
  } while (size);

  return rawDataSizeBefore - buf->size;
}

Size pagedRawDataRead(PagedRawData *buf, Position where, Byte *dest, Size size)
{
  Size currPgSpcAfterPos, readSize;
  Byte *info, *destBeginPtr;
  PageHint *pageHint;

  if (size > (buf->size - where)) {
    size = buf->size - where;
  }

  destBeginPtr = dest;
  pageHint = pageTableHint(&(buf->pageTable));

  assert(dest != NULL);
  info = dest;

  do {
    pageTableFindPage(&(buf->pageTable), where);
    currPgSpcAfterPos = rawDataSize(pageHint->pagePtr->data)
      - pageHint->offset;
    readSize = ((size < currPgSpcAfterPos) ? size : currPgSpcAfterPos);

    rawDataRead(pageHint->pagePtr->data, pageHint->offset, dest, readSize);
    dest += readSize;
    where += readSize;
    size -= readSize;
  } while (size);

  return (dest - destBeginPtr) / sizeof(Byte);
}
