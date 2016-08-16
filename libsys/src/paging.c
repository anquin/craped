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
#include "libsys/paging.h"
#include "page_factory.h"

void pageTableInit(PageTable *pageTable)
{
  pageTable->firstPage = createPage();
  pageTable->currPageHint = (PageHint *)malloc(sizeof(PageHint));
  pageTable->currPageHint->pageBegin = 0;
  pageTable->currPageHint->pageIdx = 0;
  pageTable->currPageHint->pagePtr = pageTable->firstPage;
  pageTable->currPageHint->offset = 0;
}

void pageTableDestroy(PageTable *pageTable)
{
  Page *pageToDel;

  while (pageTable->firstPage != NULL) {
    pageToDel = pageTable->firstPage;
    pageTable->firstPage = pageTable->firstPage->next;
    destroyPage(pageToDel);
  }
  pageTable->firstPage = NULL;
  free(pageTable->currPageHint);
  pageTable->currPageHint = NULL;
}

/* Adds a page to pageTable after currPage. Returns the new page. */
Page *pageTableNewPageAfter_(PageTable *pageTable)
{
  Page *prev;
  Page *next;
  Page *page;

  prev = pageTable->currPageHint->pagePtr;
  next = prev->next;
  page = createPage();

  prev->next = page;
  page->prev = prev;
  page->next = next;

  if (next != NULL)
    next->prev = page;

  return page;
}

/* Adds a page to pageTable before currPage. Returns the new page. */
Page *pageTableNewPageBefore_(PageTable *pageTable)
{
  Page *prev;
  Page *next;
  Page *page;

  prev = pageTable->currPageHint->pagePtr->prev;
  next = pageTable->currPageHint->pagePtr;
  page = createPage();

  if (prev != NULL) {
    prev->next = page;
  }
  page->prev = prev;
  page->next = next;
  pageTable->currPageHint->pagePtr->prev = page;

  pageTable->currPageHint->pageIdx += 1;

  return page;
}

void pageTableSplitPage(PageTable *pageTable)
{
  Page *oldPage, *newPage;
  Size splitSize;
  Byte *bytesToMove;

  oldPage = pageTable->currPageHint->pagePtr;

  if (pageTable->currPageHint->offset < (rawDataSize(oldPage->data) / 2)) {
    splitSize = pageTable->currPageHint->offset + 1;
    newPage = pageTableNewPageBefore_(pageTable);
    if (pageTable->currPageHint->pagePtr == pageTable->firstPage) {
      pageTable->firstPage = newPage;
    }
    if (splitSize) {
      bytesToMove = (Byte *)malloc(sizeof(Byte) * splitSize);
      rawDataRead(oldPage->data, 0, bytesToMove, splitSize);
      rawDataDelete(oldPage->data, 0, splitSize);
      rawDataInsert(newPage->data, 0, bytesToMove, splitSize);
    }

    /* Our currPageHint has become invalid. We must make it valid. */
    pageTable->currPageHint->pagePtr = newPage;
    pageTable->currPageHint->pageIdx -= 1;
  }
  else {
    splitSize = rawDataSize(oldPage->data) - pageTable->currPageHint->offset;
    newPage = pageTableNewPageAfter_(pageTable);
    if (splitSize) {
      bytesToMove = (Byte *)malloc(sizeof(Byte) * splitSize);
      rawDataRead(oldPage->data,
		  pageTable->currPageHint->offset,
		  bytesToMove,
		  splitSize);
      rawDataDelete(oldPage->data, pageTable->currPageHint->offset, splitSize);
      rawDataInsert(newPage->data, 0, bytesToMove, splitSize);
    }
  }
}

/* Deletes the specified page. */
void pageTableDeletePage_(Page *pageToDel)
{
  if (pageToDel->prev != NULL) {
    pageToDel->prev->next = pageToDel->next;
  }
  if (pageToDel->next != NULL) {
    pageToDel->next->prev = pageToDel->prev;
  }

  destroyPage(pageToDel);
}

void pageTableDeletePage(PageTable *pageTable)
{
  Page *pageToDel;
  pageToDel = pageTable->currPageHint->pagePtr;

  if (pageToDel->prev != NULL) {
    pageTable->currPageHint->pagePtr = pageToDel->prev;
    pageTable->currPageHint->pageIdx -= 1;
    pageTable->currPageHint->offset = pageTable->currPageHint->pageBegin - 1;
    pageTable->currPageHint->pageBegin -= rawDataSize(pageTable->currPageHint->pagePtr->data);
    pageTableDeletePage_(pageToDel);
  }
  else if (pageToDel->next != NULL) {
    pageTable->firstPage = pageToDel->next;
    pageTable->currPageHint->pagePtr = pageToDel->next;
    pageTable->currPageHint->pageIdx += 1;
    pageTable->currPageHint->offset = 0;
    pageTableDeletePage_(pageToDel);
  }
}

PageFindStatus pageTableFindPage(PageTable *pageTable, Position where)
{
  Position currHintDistance, pageBegin, pageEnd, pageIdx;
  short isAfter;
  Page *lastPagePtr, *pagePtr;
  PageFindStatus findStatus;

  /* Does abs(where - pageBegin) */
  if (where >= pageTable->currPageHint->pageBegin) {
    currHintDistance = where - pageTable->currPageHint->pageBegin;
    isAfter = 1;
  }
  else {
    currHintDistance = pageTable->currPageHint->pageBegin - where;
    isAfter = 0;
  }

  /* If current page is nearer to <where> then the first page we use it... */
  if (where > currHintDistance) {
    pagePtr = pageTable->currPageHint->pagePtr;
    pageBegin = pageTable->currPageHint->pageBegin;
    pageEnd = pageBegin + rawDataSize(pagePtr->data) - 1;
    pageIdx = pageTable->currPageHint->pageIdx;
  }
  else {  /* ... we use the first page otherwise. */
    pagePtr = pageTable->firstPage;
    pageBegin = 0;
    pageEnd = rawDataSize(pagePtr->data) - 1;
    pageIdx = 0;
    isAfter = 1;
  }

  /* We must stop when pageBegin <= where <= pageEnd */
  while (pageBegin > where || where > pageEnd) {
    if (isAfter) {
      if (pagePtr->next == NULL) {
	  lastPagePtr = pagePtr;
      }
      pagePtr = pagePtr->next;
      pageBegin = pageEnd + 1;
      pageIdx += 1;
    }
    else {
      pagePtr = pagePtr->prev;
      pageBegin -= rawDataSize(pagePtr->data);
      pageIdx -= 1;
    }

    if (pagePtr == NULL) {
      break;
    }

    pageEnd = pageBegin + rawDataSize(pagePtr->data) - 1;
  }

  if (pagePtr != NULL) {
    /* We found the page! */

    /* We update the currentPageHint. */
    pageTable->currPageHint->pagePtr = pagePtr;
    pageTable->currPageHint->pageBegin = pageBegin;
    pageTable->currPageHint->pageIdx = pageIdx;
    pageTable->currPageHint->offset = where - pageBegin;
    findStatus = PAGE_FOUND;
  }
  else {
    /* Page couldn't be found. */

    if (pageBegin == where) {
      /* A position after the last byte must be accessible for
	 appending purposes, so we add a new page if it is the case. */
      lastPagePtr->next = createPage();
      lastPagePtr->next->prev = lastPagePtr;
      /* We update the currentPageHint. */
      pageTable->currPageHint->pagePtr = lastPagePtr->next;
      pageTable->currPageHint->pageBegin = pageBegin;
      pageTable->currPageHint->pageIdx = pageIdx;
      pageTable->currPageHint->offset = 0;
      findStatus = PAGE_FOUND;
    }
    else {
      findStatus = PAGE_NOT_FOUND;
    }
  }

  return findStatus;
}
