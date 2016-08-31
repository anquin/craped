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
#include <stdio.h>
#include <assert.h>

#include <libsys/def.h>
#include "raw_data_test.h"

void observerUpdate(struct world_observer *wo, struct world *w)
{
}

extern Size maxPageSize;

Byte *bytesFromTxt(const char *txt)
{
  size_t len = strlen(txt);
  return memcpy(malloc(sizeof(char) * len), txt, len);
}

Byte *getBytes(TestRawData *buf)
{
  Size size, bytesRead;
  Byte *bytes;

  size = testRawDataSize(buf);
  bytes = (Byte *)malloc(sizeof(Byte) * (size + 1));
  bytesRead = testRawDataRead(buf, 0, bytes, size);
  bytes[bytesRead] = '\0';

  return bytes;
}

int main(void)
{
  const char *txt = "Selecionei frutas novas para colheita.";
  Byte *bytes;
  TestRawData buf;

  /* Sets the page size to a low value to increase the risks */
  maxPageSize = 16;

  initTestRawData(&buf);

  testRawDataInsert(&buf, 0, bytesFromTxt(txt), strlen(txt));
  bytes = getBytes(&buf);
  printf("%s --> %d bytes.\n", bytes, strlen(bytes));
  assert(!strcmp(bytes, "Selecionei frutas novas para colheita."));
  free(bytes);

  testRawDataDelete(&buf, 18, 5);
  bytes = getBytes(&buf);
  printf("%s --> %d bytes.\n", bytes, strlen(bytes));
  assert(!strcmp(bytes, "Selecionei frutas  para colheita."));
  free(bytes);

  testRawDataInsert(&buf, 18, bytesFromTxt("frescas"), 7);
  bytes = getBytes(&buf);
  printf("%s --> %d bytes.\n", bytes, strlen(bytes));
  assert(!strcmp(bytes, "Selecionei frutas frescas para colheita."));
  free(bytes);

  destroyTestRawData(&buf);
  return 0;
}
