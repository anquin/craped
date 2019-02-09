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

#ifndef WINDOW_H
#define WINDOW_H

#include <uicore/window-reference.h>
#include <subeditor.h>

struct window
{
  unsigned posX, posY;
  unsigned sizeX, sizeY;
  unsigned contentSize;
  char *bufferName;
  unsigned topLine;
  Position top;
  Mode *mode;
  struct window *prev;
  struct window *next;
  unsigned id;
  short hasStatusLine;
};

Window *
createWindow(unsigned posX, unsigned posY,
             unsigned sizeX, unsigned sizeY, unsigned id,
             Mode *mode);
void
initWindow(Window *wnd,
           unsigned posX, unsigned posY,
           unsigned sizeX, unsigned sizeY, unsigned id,
           Mode *mode);
void destroyWindow(Window *wnd);

void windowGetPosition(Window *, unsigned *x, unsigned *y);
void windowSetPosition(Window *, unsigned x, unsigned y);
void windowGetSize(Window *, unsigned *x, unsigned *y);
void windowSetSize(Window *, unsigned x, unsigned y);
char *windowGetBufferName(Window *);
void windowSetBufferName(Window *, char *);
/* returns the screen line that contains the
   top line of the specified window. */
unsigned windowGetTopLine(Window *);
void windowSetTopLine(Window *, unsigned);
Position windowGetTop(Window *);
void windowSetTop(Window *, Position);
Position windowGetBottom(Window *);
unsigned windowGetContentSize(Window *);
void windowSetContentSize(Window *, Size size);
unsigned windowGetId(Window *window);
void windowSetId(Window *window, unsigned id);
short windowHasStatusLine(Window *window);
void windowSetHasStatusLine(Window *window, short hasStatusLine);

#endif
