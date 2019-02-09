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

#include <string.h>
#include <assert.h>

#include <libsys/hashing.h>
#include <libsys/dbglog.h>

int main(void)
{
  char *keys[] = {
    "key1", "key2", "key3", "key4", "key5",
    "key6", "key7", "key8", "key9", "key10"
  };
  double values[] = {
    1.1, 2.2, 3.3, 4.4, 5.5,
    6.6, 7.7, 8.8, 9.9, 0.0
  };
  HashTable ht;
  Hasheable *k;
  int i, mod = 4;
  int n = sizeof(keys) / sizeof(char *);
  double *dptr;

  /* DBGLOG_OPEN(stdout); */
  DBGLOG("n = %d\n", n);

  initHashTable(&ht, n);

  for (i = 0; i < n; i++) {
    k = createHasheable(strHashFn, (int (*)(void *, void *))strcmp, keys[i]);
    hashTablePut(&ht, k, ((void *)(&values[i])));
    dptr = ((double *)hashTableGet(&ht, k));
    DBGLOG("%f == %f\n", *dptr, values[i]);
    assert(*dptr == values[i]);
  }
  for (i = 0; i < n; i++) {
    k = createHasheable(strHashFn, (int (*)(void *, void *))strcmp, keys[i]);
    dptr = ((double *)hashTableGet(&ht, k));
    DBGLOG("%f == %f\n", *dptr, values[i]);
    assert(*dptr == values[i]);
  }

  destroyHashTable(&ht);
  DBGLOG_CLOSE();
  return 0;
}
