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

#include "libsys/quicksearch.h"

#include <stdlib.h>
#include <string.h>

void preQsBcForward(Byte *pat, Size m, Size qsBc[])
{
  Position i;

  for (i = 0; i < ALPHABET_SIZE; i++) {
    qsBc[i] = m + 1;
  }
  for (i = 0; i < m; i++) {
    qsBc[pat[i]] = m - i;
  }
}

Position
quickSearchForward(Byte *pat, Size m, Byte *content, Size n, Size qsBc[])
{
  Position j;

  j = 0;
  while (j <= n - m) {
    if (memcmp(pat, content + j, m) == 0) {
      return j;
    }
    j += qsBc[content[j + m]];
  }

  return n;
}
