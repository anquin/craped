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

#include "mark.h"
#include <string.h>
#include <stdlib.h>

#include <libsys/mem.h>
#include <libsys/strhash.h>

void initMark(Mark *mark, char *name, Position bytePos, Byte flags)
{
  strncpy(mark->name, name, MARK_NAME_MAX);
  mark->bytePos = bytePos;
  mark->flags = flags;
}

Mark *createMark(char *name, Position bytePos, Byte flags)
{
  Mark *nmark;

  nmark = (Mark *)malloc(sizeof(Mark));
  initMark(nmark, name, bytePos, flags);

  return nmark;
}

void destroyMark(Mark *mark)
{
  free(mark);
}

void initMarkChain(MarkChain *markChain, unsigned mod)
{
  markChain->list = NULL;
  hash_table_init(&markChain->hashTable, mod);
}

void markChainPushFront(MarkChain *markChain, char *name, Position bytePos, Byte flags)
{
  Mark *m;
  Byte markFlags;
  Hashed *key;

  m = createMark(name, bytePos, flags);
  m->next = markChain->list;
  markChain->list = m;
  strncpy(m->name, name, MARK_NAME_MAX);

  key = hash_string(lsnew(Hashed), (char *)(name));

  hash_table_put(&markChain->hashTable, key, m);
}

void markChainRemove(MarkChain *markChain, char *nome)
{
  Hashed key;

  hash_string(&key, (char *)(nome));
  hash_table_remove(&markChain->hashTable, &key);
  hashed_fini(&key);
}

void *markChainFind(MarkChain *markChain, char *nome)
{
  Hashed key;
  void *info;

  hash_string(&key, (char *)(nome));
  info = hash_table_get(&markChain->hashTable, &key);
  hashed_fini(&key);

  return info;
}
