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

#ifndef DIFFEX_FIFO_H
#define DIFFEX_FIFO_H

#include <diffexui/diffex.h>
#include <subeditor/def.h>

void createDEFifo(Diffex *diffex, unsigned capacity);
void initDEFifo(Diffex *diffex, unsigned capacity);
void deFifoPopPush(Diffex *diffex, Byte *oldByte, Byte *newByte);
void deFifoPopPushInt(Diffex *diffex, int *oldInt, int *newInt);
void deFifoPopPushUInt(Diffex *diffex, unsigned *oldInt, unsigned *newInt);
#define deFifoPopPushChar(deFifo, oldChar, newChar) (deFifoPopPush(deFifo, (Byte *)oldChar, (Byte *newChar)))
void deFifoPopPushStrN(Diffex *diffex, char *oldStr, char *newStr,
                       unsigned *oldN, unsigned *newN);
void deFifoSwap(Diffex *diffex);
void destroyDEFifo(Diffex *diffex);

/* Use those functions below carefully,
   always remember to invoke the push() immediately after a pop() */
void deFifoPopUInt(Diffex *diffex, unsigned *oldInt);
void deFifoPushUInt(Diffex *diffex, unsigned *newInt);
void deFifoPopStrN(Diffex *diffex, char *oldStr, unsigned *oldN);
void deFifoPushStrN(Diffex *diffex, char *newStr, unsigned *newN);

#endif
