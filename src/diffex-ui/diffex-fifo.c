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

#include "diffex-fifo.h"

#include <stdlib.h>

#include <libsys/dbglog.h>

struct de_fifo
{
  Byte *oldInfo;
  Byte *newInfo;
  unsigned oldI;
  unsigned newI;
  unsigned capacity;
};

void createDEFifo(Diffex *diffex, unsigned capacity)
{
  diffex->fifo = (DEFifo *)malloc(sizeof(DEFifo));
  initDEFifo(diffex, capacity);
}

void initDEFifo(Diffex *diffex, unsigned capacity)
{
  diffex->fifo->oldInfo = (Byte *)malloc(sizeof(Byte) * capacity);
  diffex->fifo->newInfo = (Byte *)malloc(sizeof(Byte) * capacity);
  diffex->fifo->oldI = diffex->fifo->newI = 0;
  diffex->fifo->capacity = capacity;
}

void deFifoPopPush(Diffex *diffex, Byte *oldByte, Byte *newByte)
{
  if (diffex->mode & DE_ERASE) {
    *oldByte = diffex->fifo->oldInfo[diffex->fifo->oldI];
    ++(diffex->fifo->oldI);
  }
  if (diffex->mode & DE_SHOW) {
    if (diffex->fifo->newI >= diffex->fifo->capacity) {
      unsigned newCap;
      newCap = diffex->fifo->capacity << 1;
      diffex->fifo->oldInfo = realloc(diffex->fifo->oldInfo, newCap);
      diffex->fifo->newInfo = realloc(diffex->fifo->newInfo, newCap);
      diffex->fifo->capacity = newCap;
    }
    diffex->fifo->newInfo[diffex->fifo->newI] = *newByte;
    ++(diffex->fifo->newI);
  }
}

void deFifoPopPushInt(Diffex *diffex, int *oldInt, int *newInt)
{
  int i; Byte *oldPtr = (Byte *)oldInt, *newPtr = ((Byte*)newInt);
  if (diffex->mode == DE_SHOW) {
    for (i = 0; i < sizeof(int); i++) {
      deFifoPopPush(diffex, NULL, newPtr + i);
    }
  }
  else if (diffex->mode == DE_ERASE) {
    for (i = 0; i < sizeof(int); i++) {
      deFifoPopPush(diffex, oldPtr + i, NULL);
    }
  }
  else {
    for (i = 0; i < sizeof(int); i++) {
      deFifoPopPush(diffex, oldPtr + i, newPtr + i);
    }
  }
  DBGLOG("deFifoPopPushInt(%d, %d) %s\n", (oldInt ? *oldInt : -1), (newInt ? *newInt : -1), de_modes[diffex->mode - 1]);
}

void deFifoPopPushUInt(Diffex *diffex, unsigned *oldUInt, unsigned *newUInt)
{
  int i; Byte *oldPtr = (Byte *)oldUInt, *newPtr = ((Byte*)newUInt);
  if (diffex->mode == DE_SHOW) {
    for (i = 0; i < sizeof(unsigned); i++) {
      deFifoPopPush(diffex, NULL, newPtr + i);
    }
  }
  else if (diffex->mode == DE_ERASE) {
    for (i = 0; i < sizeof(unsigned); i++) {
      deFifoPopPush(diffex, oldPtr + i, NULL);
    }
  }
  else {
    for (i = 0; i < sizeof(unsigned); i++) {
      deFifoPopPush(diffex, oldPtr + i, newPtr + i);
    }
  }
  DBGLOG("deFifoPopPushUInt(%d, %d)%s \n", (oldUInt ? *oldUInt : -1), (newUInt ? *newUInt : -1), de_modes[diffex->mode - 1]);
}

void deFifoPopPushStrN(Diffex *diffex, char *oldStr, char *newStr,
                       unsigned *oldN, unsigned *newN)
{
  unsigned i;
  if (diffex->mode == DE_SHOW) {
    deFifoPopPushUInt(diffex, NULL, newN);
    for (i = 0; i < *newN; i++) {
      deFifoPopPush(diffex, NULL, newStr + i);
    }
  }
  else if (diffex->mode == DE_ERASE) {
    deFifoPopPushUInt(diffex, oldN, NULL);
    for (i = 0; i < *oldN; i++) {
      deFifoPopPush(diffex, oldStr + i, NULL);
    }
  }
  else {
    int m1;
    m1 = diffex->mode;
    deFifoPopPushUInt(diffex, oldN, newN);
    diffex->mode = DE_ERASE;
    for (i = 0; i < *oldN; i++) {
      deFifoPopPush(diffex, oldStr + i, NULL);
    }
    diffex->mode = DE_SHOW;
    for (i = 0; i < *newN; i++) {
      deFifoPopPush(diffex, NULL, newStr + i);
    }
    diffex->mode = m1;
  }
  DBGLOG("deFifoPopPushStrN(%.*s, %.*s) %s\n",
         (*oldN ? *oldN : 1), (*oldN ? oldStr : "N"),
         (*newN ? *newN : 1), (*newN ? newStr : "N"),
         de_modes[diffex->mode - 1]);
}

void deFifoSwap(Diffex *diffex)
{
  Byte *aux;
  aux = diffex->fifo->oldInfo;
  diffex->fifo->oldInfo = diffex->fifo->newInfo;
  diffex->fifo->newInfo = aux;
  diffex->fifo->oldI = 0;
  diffex->fifo->newI = 0;
}

void destroyDEFifo(Diffex *diffex)
{
  free(diffex->fifo->oldInfo);
  free(diffex->fifo->newInfo);
}


void deFifoPopUInt(Diffex *diffex, unsigned *oldUInt)
{
  if (diffex->mode != DE_SHOW) {
    int oldMode;
    oldMode = diffex->mode;
    diffex->mode = DE_ERASE;
    deFifoPopPushUInt(diffex, oldUInt, NULL);
    diffex->mode = oldMode;
  }
}

void deFifoPushUInt(Diffex *diffex, unsigned *newUInt)
{
  if (diffex->mode != DE_ERASE) {
    int oldMode;
    oldMode = diffex->mode;
    diffex->mode = DE_SHOW;
    deFifoPopPushUInt(diffex, NULL, newUInt);
    diffex->mode = oldMode;
  }
}

void deFifoPopStrN(Diffex *diffex, char *oldStr, unsigned *oldN)
{
  if (diffex->mode != DE_SHOW) {
    int oldMode;
    oldMode = diffex->mode;
    diffex->mode = DE_ERASE;
    deFifoPopPushStrN(diffex, oldStr, NULL, oldN, NULL);
    diffex->mode = oldMode;
  }
}

void deFifoPushStrN(Diffex *diffex, char *newStr, unsigned *newN)
{
  if (diffex->mode != DE_ERASE) {
    int oldMode;
    oldMode = diffex->mode;
    diffex->mode = DE_SHOW;
    deFifoPopPushStrN(diffex, NULL, newStr, NULL, newN);
    diffex->mode = oldMode;
  }
}
