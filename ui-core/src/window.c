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

#include <uicore/window.h>
#include <uicore/keys.h>
#include <subeditor.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

Window *
createWindow(unsigned posX, unsigned posY,
             unsigned sizeX, unsigned sizeY, unsigned id,
             Mode *mode)
{
  Window *window;

  window = (Window *)malloc(sizeof(Window));
  initWindow(window, posX, posY, sizeX, sizeY, id, mode);

  return window;
}

void
initWindow(Window *wnd,
           unsigned posX, unsigned posY,
           unsigned sizeX, unsigned sizeY, unsigned id,
           Mode *mode)
{
  wnd->posX = posX;
  wnd->posY = posY;
  wnd->sizeX = sizeX;
  wnd->sizeY = sizeY;
  wnd->mode = mode;
  wnd->top = 0;
  wnd->prev = wnd->next = wnd;
  wnd->bufferName = NULL;
  wnd->contentSize = 0;
  wnd->id = id;
  wnd->hasStatusLine = 0;
}

void destroyWindow(Window *wnd)
{
}

void windowGetPosition(Window *wnd, unsigned *x, unsigned *y)
{
  *x = wnd->posX;
  *y = wnd->posY;
}

void windowSetPosition(Window *wnd, unsigned x, unsigned y)
{
  wnd->posX = x;
  wnd->posY = y;
}

void windowGetSize(Window *wnd, unsigned *x, unsigned *y)
{
  *x = wnd->sizeX;
  *y = wnd->sizeY;
}

void windowSetSize(Window *wnd, unsigned x, unsigned y)
{
  wnd->sizeX = x;
  wnd->sizeY = y;
}

char *windowGetBufferName(Window *wnd)
{
  return wnd->bufferName;
}

void windowSetBufferName(Window *wnd, char *bufName)
{
  wnd->bufferName = (char *)realloc(wnd->bufferName,
                                    sizeof(char) * strlen(bufName));
  strcpy(wnd->bufferName, bufName);
}

unsigned windowGetTopLine(Window *wnd)
{
  return wnd->topLine;
}

void windowSetTopLine(Window *wnd, unsigned topLine)
{
  wnd->topLine = topLine;
}

Position windowGetTop(Window *wnd)
{
  return wnd->top;
}

void windowSetTop(Window *wnd, Position top)
{
  wnd->top = top;
}

Position windowGetBottom(Window *wnd)
{
  return wnd->top + wnd->contentSize;
}

unsigned windowGetContentSize(Window *window)
{
  return window->contentSize;
}

void windowSetContentSize(Window *window, Size size)
{
  window->contentSize = size;
}

void
windowSetMode(Window *window, Mode *mode)
{
  window->mode = mode;
}

Mode *windowGetMode(Window *window)
{
  return window->mode;
}

unsigned windowGetId(Window *window)
{
  return window->id;
}

void windowSetId(Window *window, unsigned id)
{
  window->id = id;
}

short windowHasStatusLine(Window *window)
{
  return window->hasStatusLine;
}

void windowSetHasStatusLine(Window *window, short hasStatusLine)
{
  window->hasStatusLine = hasStatusLine;
}
