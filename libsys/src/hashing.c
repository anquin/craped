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

#include <libsys/hashing.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void initHasheable(Hasheable *hasheable, unsigned (*hashfn)(void *, unsigned),
		   int (*equalsfn)(void *, void *), void *info)
{
  hasheable->hashfn = hashfn;
  hasheable->equalsfn = equalsfn;
  hasheable->info = info;
}

Hasheable *createHasheable(unsigned (*hashfn)(void *, unsigned),
			   int (*equalsfn)(void *, void *), void *info)
{
  Hasheable *hasheable;

  hasheable = (Hasheable *)malloc(sizeof(Hasheable));
  initHasheable(hasheable, hashfn, equalsfn, info);

  return hasheable;
}

HashNode *createHashNode(Hasheable *key, void *info, HashNode *next)
{
  HashNode *node;

  node = (HashNode *)malloc(sizeof(HashNode));
  node->key = key;
  node->info = info;
  node->next = next;
}

void initHashTable(HashTable *hashTable, unsigned mod)
{
  hashTable->table = (HashNode **)malloc(sizeof(HashNode *) * mod);
  memset(hashTable->table, 0, sizeof(HashNode *) * mod);
  hashTable->mod = mod;
}

void destroyHashTable(HashTable *hashTable)
{
  /* TODO */
}

void hashTablePut(HashTable *hashTable, Hasheable *key, void *info)
{
  unsigned pos;

  pos = hasheableHashFn(key, hashTable->mod);

  if (hashTable->table[pos] == NULL) {
    hashTable->table[pos] = createHashNode(key, info, NULL);
  }
  else {
    hashTable->table[pos] = createHashNode(key, info, hashTable->table[pos]);
  }
}

void *hashTableGet(HashTable *hashTable, Hasheable *key)
{
  HashNode *node;

  node = hashTable->table[hasheableHashFn(key, hashTable->mod)];

  while (node != NULL) {
    if (!hasheableEqualsFn(key, node->key->info)) {
      break;
    }
    node = node->next;
  }

  if (node == NULL) {
    return NULL;
  }

  return node->info;
}

void hashTableRemove(HashTable *hashTable, Hasheable *key)
{
  HashNode *prev;
  HashNode *node;
  unsigned hash = hasheableHashFn(key, hashTable->mod);

  node = hashTable->table[hash];

  if (!hasheableEqualsFn(key, node->key->info)) {
    hashTable->table[hash] = node->next;
    free(node);
    return;
  }

  prev = node;

  while (prev != NULL) {
    if (prev->next != NULL) {
      if (!hasheableEqualsFn(key, prev->next)) {
	break;
      }
    }
    prev = prev->next;
  }

  if (prev != NULL) {
    node = prev->next;
    prev->next = prev->next->next;
    free(node);
  }
}

unsigned strHashFn(void *s, unsigned mod)
{
  unsigned hash;
  char *str = (char *)s;

  hash = 0;
  while (*(str++)) {
    hash += (unsigned)(31 * (*str));
  }
  hash %= mod;

  return hash;
}

unsigned hasheableHashFn(Hasheable *hasheable, unsigned mod) {return hasheable->hashfn(hasheable->info, mod);}
int hasheableEqualsFn(Hasheable *hasheable, void *info) {return hasheable->equalsfn(hasheable->info, info);}
