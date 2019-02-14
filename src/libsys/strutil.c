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
#include <libsys/strutil.h>
#include <libsys/mem.h>

char *copy_str(char *s)
{
  return copy_str_n(s, strlen(s));
}

char *copy_str_n(char *s, size_t n)
{
  return copy_str_n_cb(s, n, NULL);
}

char *copy_str_cb(char *s, char (*cb)(char *s, size_t *i))
{
  return copy_str_n_cb(s, strlen(s), NULL);
}

char *copy_str_n_cb(char *s, size_t n, char (*cb)(char *s, size_t *i))
{
  size_t i;
  char *cpy, *ptr;
  if (s == NULL) return NULL;
  ptr = cpy = lsmalloc(sizeof(*s) * n + 1);
  for (i = 0; s[i] != '\0' && i < n; i++) {
    if (cb != NULL) *ptr = cb(s, &i);
    else *ptr = s[i];
    ++ptr;
  }
  *ptr = '\0';
  return cpy;
}
