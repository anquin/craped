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

#include <dbg/print_page_table.h>

#include <stdlib.h>


void printPageHint(PageTable *pageTable, FILE *out)
{
  Page *pagePtr = pageTable->firstPage;

  fprintf(out, "pageIdx=%u\npageBegin=%u\noffset=%u\npagePtr=%x",
	  pageTable->currPageHint->pageIdx, pageTable->currPageHint->pageBegin,
	  pageTable->currPageHint->offset, pageTable->currPageHint->pagePtr);
}

void printPageInfo(Page *currPage, FILE *out)
{
  unsigned char *dataStr;
  Size dataSize;

  dataSize = rawDataSize(currPage->data);
  dataStr = (unsigned char *)malloc(sizeof(char) * dataSize + 1);
  rawDataRead(currPage->data, 0, dataStr, dataSize);
  dataStr[dataSize] = '\0';

  fprintf(out, "data=%s\ndataSize=%u\ngapSize=%u\nprev=%x\nnext=%x\npagePtr=%x",
	  dataStr, dataSize, currPage->data->gapSize, currPage->prev, currPage->next, currPage);
}

void printPageTable(PageTable *pageTable, FILE *out)
{
  Page *currPage;

  fprintf(out, "==-Curr Page Hint-==\n");
  printPageHint(pageTable, out);

  currPage = pageTable->firstPage;
  while (currPage != NULL) {
    fprintf(out, "\n\n");
    printPageInfo(currPage, out);
    currPage = currPage->next;
  }

  fprintf(out, "\n\n");
}
