/*
 * Copyright (C) 2016 Andre Q. Barbosa
 *
 * This file is part of NoNameYet.
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
#include <libsys/mem.h>

void *lsmalloc(size_t size)
{
  void *ptr;
  /* printf("Mallocating %u bytes...", size); */
  ptr = malloc(size);
  /* printf(" Mallocated: %x.\n", ptr); */
  return ptr;
}

void lsfree(void *ptr)
{
  /* printf("Freeing %x...", ptr); */
  free(ptr);
  /* printf(" Freed %x.\n", ptr); */
}

void *lsrealloc(void *ptr, size_t size)
{
  /* printf("Reallocating %u bytes...", size); */
  ptr = realloc(ptr, size);
  /* printf(" Reallocated: %x.\n", ptr); */
  return ptr;
}
