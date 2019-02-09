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

typedef struct hasheable
{
  unsigned (*hashfn)(void *, unsigned);
  int (*equalsfn)(void *, void *);

  void *info;
} Hasheable;

void initHasheable(Hasheable *hasheable, unsigned (*hashfn)(void *, unsigned),
		   int (*equalsfn)(void *, void *), void *);
Hasheable *createHasheable(unsigned (*hashfn)(void *, unsigned),
			   int (*equalsfn)(void *, void *), void *info);
void destroyHasheable(Hasheable *hasheable);
unsigned hasheableHashFn(Hasheable *hasheable, unsigned mod);
int hasheableEquaslFn(Hasheable *hasheable, void *info);
unsigned strHashFn(void *s, unsigned mod);

typedef struct hash_node
{
  Hasheable *key;
  void *info;
  struct hash_node *next;
} HashNode;

typedef struct hash_table
{
  unsigned mod;
  HashNode **table;
} HashTable;

void initHashTable(HashTable *hashTable, unsigned mod);
void destroyHashTable(HashTable *hashTable);
void hashTablePut(HashTable *hashTable, Hasheable *key, void *info);
void *hashTableGet(HashTable *hashTable, Hasheable *key);
void hashTableRemove(HashTable *hashTable, Hasheable *key);
//Iterator *hashTableGetIterator(HashTable *hashTable);
#define hashTableMod(hashTable) (hashTable->mod)

#endif
