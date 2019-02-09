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
#include "page_factory.h"

#ifndef PAGED_RAW_DATA_MAX_PAGE_SIZE
/* TODO: Don't use this default!. It's low for testing purposes */
#define PAGED_RAW_DATA_MAX_PAGE_SIZE 16
#endif
/* TODO: "(...) the buffer page size should be set the same or as a multiple
 * of the virtual memory page size. Thus, in even the tightest memory
 * situations an insert or delete of one character will only affect at most
 * two pages of buffer memory." (Finseth) */
Size maxPageSize = PAGED_RAW_DATA_MAX_PAGE_SIZE;

Page *createPage(void)
{
  Page *page = (Page *)malloc(sizeof(Page));

  page->data = newRawData(maxPageSize);
  page->prev = NULL;
  page->next = NULL;

  return page;
}

void destroyPage(Page *page)
{
  rawDataDestroy(page->data);
  page->prev = NULL;
  page->next = NULL;
  free(page); /* Is it the best choice? See TODO. */
}
