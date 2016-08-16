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

#ifndef MODE_H
#define MODE_H

#include "subeditor.h"

/* Defines a callback function for moving the cursor in the buffer.
   Returns the number of units(eg.: chars) skipped. */
typedef void (*MoveBufferPointFn)(World *, int displ);

typedef struct mode
{
  MoveBufferPointFn moveBufferPointFn;
  MoveBufferPointFn moveWordBufferPointFn;
  MoveBufferPointFn moveLineBufferPointFn;
} Mode;

void
initMode(Mode *mode,
         MoveBufferPointFn moveBufferPointFn,
         MoveBufferPointFn moveWordBufferPointFn,
         MoveBufferPointFn moveLineBufferPointFn);

#define destroyMode(mode) ()

#define modeGetBufferPointFn(mode)     (((Mode *)mode)->moveBufferPointFn)
#define modeGetBufferPointWordFn(mode) (((Mode *)mode)->moveWordBufferPointFn)
#define modeGetBufferPointLineFn(mode) (((Mode *)mode)->moveLineBufferPointFn)

#endif
