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

#include <libsys/raw_data.h>
#include "../src/raw_data_private.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <libsys/def.h>

void observerUpdate(struct world_observer *wo, struct world *w)
{
}

int main(void)
{
  RawData rawData;
  Size contentSz;
  Position pos;
  Byte content[] = "tralalatutitrelele";
  Byte pat[] = "tuti";
  Position i, j;
  Byte *proof;
  Size patLen;

  patLen = strlen(pat);

  contentSz = strlen(content);
  rawDataInit(&rawData, 101);

  rawDataInsert(&rawData, 0, content, contentSz);
  rawDataMoveGap_(&rawData, 9);

  pos = rawDataSearchForward(&rawData, pat, patLen, 0);

  printf("%s\n", content);
  for (i = 0, j = 0; i < pos; i++) {
    if (mblen(content + j, i - j) >= 0) {
      j = i;
      printf(" ");
    }
  }
  printf("^\n");

  proof = (Byte *)malloc(sizeof(Byte) * patLen);
  rawDataRead(&rawData, pos, proof, patLen);
  assert(!strncmp(pat, proof, patLen));

  return 0;
}
