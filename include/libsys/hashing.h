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

#ifndef LIBSYS_HASHING_H
#define LIBSYS_HASHING_H

typedef unsigned Hash;

typedef struct hashed
{
  int (*equalsfn)(void *, void *);
  Hash hash;
  void *info;
} Hashed;

Hashed *hashed_init(Hashed *hashed, Hash hash, void *info,
                    int (*eq_fn)(void *, void *));
Hashed *hashed_fini(Hashed *hashed);
int hashed_equals(Hashed *hashed, void *info);

typedef struct hash_node
{
  Hashed *key;
  void *info;
  struct hash_node *next;
} HashNode;

typedef struct hash_table
{
  unsigned mod;
  HashNode **table;
} HashTable;

void hash_table_init(HashTable *hashTable, unsigned mod);
void hash_table_fini(HashTable *hashTable);
void hash_table_put(HashTable *hashTable, Hashed *key, void *info);
void *hash_table_get(HashTable *hashTable, Hashed *key);
void hash_table_remove(HashTable *hashTable, Hashed *key);
//Iterator *hashTableGetIterator(HashTable *hashTable);
#define hash_table_mod(hashTable) (hashTable->mod)

#endif
