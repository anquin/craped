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

#include "redisplay-utils.h"

#include <stdlib.h>

void
fetchBytes(World *world,
           MoveBufferPointFn moveFn,
           char *line,
           unsigned *nbytes)
{
  Position beg, end;
  Size sz;
  *nbytes = 0;
  beg = worldGetPoint(world);
  if (beg >= worldBufferSize(world)) {
    return;
  }
  moveFn(world, 1);
  end = worldGetPoint(world);
  worldSetPoint(world, beg);
  sz = end - beg;
  worldGetChunk(world, (Byte *)line, sz);
  worldSetPoint(world, end);
  *nbytes = sz;
}

int
textDiff(char **t1, char **t2,
         unsigned *t1sz, unsigned *t2sz,
         unsigned *diff1sz, unsigned *diff2sz,
         TextWalker *walker)
{
  int stepCount, nbytes, i;
  *diff1sz = 0;
  *diff2sz = 0;

  stepCount = 0;
  while (1) {
    nbytes = textWalkerWalk(walker, t1, 1);
    if (!nbytes) {
      *diff1sz = *t1sz;
      *diff2sz = *t2sz;
      return stepCount;
    }
    for (i = 0; i < nbytes; i++) {
      if (*t1sz == 0 || *t2sz == 0) {
        *diff1sz = *t1sz;
        *diff2sz = *t2sz;
        return stepCount;
      }
      if (**t1 != **t2) {
        *diff1sz = *t1sz;
        *diff2sz = *t2sz;
        return stepCount;
      }
      ++(*t1);
      ++(*t2);
      --(*t1sz);
      --(*t2sz);
    }
    ++stepCount;
  }
}

int
textNextDiff(char **t1, char **t2,
             unsigned *t1sz, unsigned *t2sz,
             unsigned *diff1sz, unsigned *diff2sz,
             TextWalker *walker)
{
  *t1 += *diff1sz;
  *t1sz -= *diff1sz;
  *t2 += *diff2sz;
  *t2sz -= *diff2sz;
  return textDiff(t1, t2, t1sz, t2sz, diff1sz, diff2sz, walker);
}


#include <subeditor.h>

int textWalkerWalk(TextWalker *walker, char **s, int steps)
{
  register unsigned oldPos, newPos;
  int nbytes;
  oldPos = worldGetPoint(walker->world);
  if ((steps > 0) && (oldPos + steps > worldBufferSize(walker->world))) {
    return 0;
  }
  if ((steps < 0) && ((steps * -1) > oldPos)) {
    return 0;
  }
  walker->movePoint(walker->world, steps);
  newPos = worldGetPoint(walker->world);
  nbytes = newPos - oldPos;
  return (int)(nbytes);
}

void
initTextWalker(TextWalker *walker, World *world, MoveBufferPointFn movePoint)
{
  walker->world = world;
  walker->movePoint = movePoint;
  worldAddMark(world, "walkerbeg");
}

void destroyTextWalker(TextWalker *walker)
{
  worldMarkToPoint(walker->world, "walkerbeg");
  worldRemoveMark(walker->world, "walkerbeg");
}

int glyphSize_(char c[], int nbytes)
{
  if ((nbytes == 1) && (c[0] == '\n')) {
    return 0;
  }
  if ((nbytes == 1) && (c[0] == '\t')) {
    return 2;
  }
  return 1;
}
