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

#ifndef BUFFER_PAGING_H
#define BUFFER_PAGING_H

#include "libsys/def.h"
#include "raw_data.h"

typedef enum PageFindStatus { PAGE_NOT_FOUND, PAGE_FOUND } PageFindStatus;

typedef struct buffer_page {
  RawData *data;
  struct buffer_page *prev;
  struct buffer_page *next;
} Page;

/* Stores a page info helping to find near
   pages to avoid much calculation. */
typedef struct page_hint
{
  /* Position of the first byte of a page in the buffer */
  Position pageBegin;

  /* Page Index */
  unsigned pageIdx;

  /* Reference to the page */
  Page *pagePtr;

  /* Last search page offset */
  Position offset;
} PageHint;

typedef struct raw_data_page_table {
  Page *firstPage;
  PageHint *currPageHint;
} PageTable;

#define pageTableHint(pageTable) ((pageTable)->currPageHint)

/* Inits PageTable. current Page's Hint is set to first page. */
void pageTableInit(PageTable *pageTable);

/* Finalizes a PageTable. */
void pageTableDestroy(PageTable *pageTable);

/* Splits the currPage in two at the current page's
   hint offset, updates the current Page's Hint. */
void pageTableSplitPage(PageTable *pageTable);

/* Deletes the current page if it is not the only one.
   Updates the current Page's Hint. */
void pageTableDeletePage(PageTable *pageTable);

/* Tries to find a page given a position.
   The current Page's Hint is used to aux the searching. */
PageFindStatus pageTableFindPage(PageTable *pageTable, Position where);

/* TODO: Improve and test this */
#define pageTableGoToNextPage(pageTable) (pageTableFindPage(pageTable, (pageTable)->curPageHint->pageBegin + rawDataSize((pageTable)->currPageHint->pagePtr)))

#endif
