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

Hashed *hashed_init(Hashed *hashed, Hash hash, void *info,
                    int (*eq_fn)(void *, void *))
{
  hashed->hash = hash;
  hashed->equalsfn = eq_fn;
  hashed->info = info;
  return hashed;
}

Hashed *hashed_fini(Hashed *hashed)
{
  /* Does nohing */
  return hashed;
}

static HashNode *createHashNode(Hashed *key, void *info, HashNode *next)
{
  HashNode *node;

  node = (HashNode *)malloc(sizeof(HashNode));
  node->key = key;
  node->info = info;
  node->next = next;

  return node;
}

void hash_table_init(HashTable *hashTable, unsigned mod)
{
  hashTable->table = (HashNode **)malloc(sizeof(HashNode *) * mod);
  memset(hashTable->table, 0, sizeof(HashNode *) * mod);
  hashTable->mod = mod;
}

void hash_table_fini(HashTable *hashTable)
{
  /* TODO */
}

#define hash_to_idx(hash, mod) (hash % mod)

void hash_table_put(HashTable *hashTable, Hashed *key, void *info)
{
  unsigned pos;
  pos = hash_to_idx(key->hash, hashTable->mod);

  if (hashTable->table[pos] == NULL) {
    hashTable->table[pos] = createHashNode(key, info, NULL);
  }
  else {
    hashTable->table[pos] = createHashNode(key, info, hashTable->table[pos]);
  }
}

void *hash_table_get(HashTable *hashTable, Hashed *key)
{
  HashNode *node;
  node = hashTable->table[hash_to_idx(key->hash, hashTable->mod)];

  while (node != NULL) {
    if (!hashed_equals(key, node->key->info)) {
      break;
    }
    node = node->next;
  }

  if (node == NULL) {
    return NULL;
  }

  return node->info;
}

void hash_table_remove(HashTable *hashTable, Hashed *key)
{
  HashNode *prev;
  HashNode *node;
  unsigned pos = hash_to_idx(key->hash, hashTable->mod);

  node = hashTable->table[pos];

  if (!hashed_equals(key, node->key->info)) {
    hashTable->table[pos] = node->next;
    free(node);
    return;
  }

  prev = node;

  while (prev != NULL) {
    if (prev->next != NULL) {
      if (!hashed_equals(key, prev->next)) {
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

int hashed_equals(Hashed *hashed, void *info) {return hashed->equalsfn(hashed->info, info);}
