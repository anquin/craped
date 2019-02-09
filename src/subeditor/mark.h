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

#ifndef MARK_INTERNAL_H
#define MARK_INTERNAL_H

#include "subeditor/def.h"
#include <libsys/hashing.h>

#define MARK_NAME_MAX 32

#define MARK_FLAG_FIXED 1

typedef struct mark
{
  struct mark *next;
  char name[MARK_NAME_MAX];
  Position bytePos;
  Byte flags;
} Mark;

void initMark(Mark *mark, char *name, Position bytePos, Byte flags);
Mark *createMark(char *name, Position bytePos, Byte flags);
void destroyMark(Mark *mark);
#define markGetPosition(m) (m->bytePos)
#define markSetPosition(m, pos) (m->bytePos = pos)

typedef struct mark_chain
{
  HashTable hashTable;
  Mark *list;
} MarkChain;

void initMarkChain(MarkChain *markChain, unsigned mod);
void *markChainFind(MarkChain *markChain, char *name);
void markChainRemove(MarkChain *markChain, char *name);
void markChainPushFront(MarkChain *markChain, char *name, Position bytePos, Byte flags);

#endif
