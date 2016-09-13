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

#ifndef REDISPLAY_UTILS_H
#define REDISPLAY_UTILS_H

#include <subeditor.h>
#include <subeditor/mode.h>

#include <stdlib.h>

typedef struct text_walker TextWalker;
void destroyTextWalker(TextWalker *walker);
int textWalkerWalk(TextWalker *walker, char **s, int steps);

/*
 * Fetches the text from current position until the
 * end of the displacement done by the function moveFn.
 * Return the total of chars fetched.
*/
void
fetchBytes(World *world,
           MoveBufferPointFn moveFn,
           char *line,
           unsigned *nbytes);

/*
 * Find the first different hunk of two strings.
 * Obs.: actually changes t1 and t2, so be careful.
 *
 * Params:
 * t1 - text 1  [I/O]
 * t2 - text 2  [I/O]
 * t1sz - size of text 1  [I/O]
 * t2sz - size of text 2  [I/O]
 * diff1sz - size of the different hunk of text 1  [O]
 * diff2sz - size of the different hunk of text 2  [O]
 *
 * Returns the glyph size up until the difference.
*/
int
textDiff(char **t1, char **t2,
         unsigned *t1sz, unsigned *t2sz,
         unsigned *diff1sz, unsigned *diff2sz,
         TextWalker *walker);

/*
 * Find the next different hunk of two strings; must be called after textDiff()
 * Obs.: actually changes t1 and t2, so be careful.
 *
 * Params:
 * t1 - text 1  [I/O]
 * t2 - text 2  [I/O]
 * t1sz - size of text 1  [I/O]
 * t2sz - size of text 2  [I/O]
 * diff1sz - size of the different hunk of text 1  [O]
 * diff2sz - size of the different hunk of text 2  [O]
 *
 * Returns the glyph size up until the difference.
 */
int
textNextDiff(char **t1, char **t2,
             unsigned *t1sz, unsigned *t2sz,
             unsigned *diff1sz, unsigned *diff2sz,
             TextWalker *walker);

#include <uicore/terminal.h>

struct text_walker
{
  World *world;
  MoveBufferPointFn movePoint;
};

void
initTextWalker(TextWalker *walker, World *world, MoveBufferPointFn movePoint);


/* FIXME: How to handle different encodings? Is it needed? */
int glyphSizeOfSingleChar_(char c[], int nbytes);

#endif
