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
#include <uicore/windowmanager.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <libsys/dbglog.h>

void resetWindowIds_(WindowManager *windowManager)
{
  int i;
  for (i = 1; i <= WNDMAN_MAX_WINDOWS; i++) {
    windowManager->unusedIds[i-1] = i;
  }
}

unsigned generateWindowId_(WindowManager *wndMan)
{
  unsigned id;
  int i;
  i = 0;
  while (i < WNDMAN_MAX_WINDOWS && !(wndMan->unusedIds[i])) {
    ++i;
  }
  id = wndMan->unusedIds[i];
  wndMan->unusedIds[i] = 0;
  DBGLOG("Generated WindowID %d\n", id);
  return id;
}

void freeWindowId_(WindowManager *wndMan, unsigned id)
{
  wndMan->unusedIds[id - 1] = id;
  DBGLOG("Freed WindowID %d\n", id);
}

void
initWindowManager(WindowManager *windowManager)
{
  unsigned winId;
  resetWindowIds_(windowManager);
  windowManager->sizeX = WNDMAN_MAX_SPLITS_H;
  windowManager->sizeY = WNDMAN_MAX_SPLITS_V;
  winId = generateWindowId_(windowManager);
  windowManager->window = createWindow(0, 0, windowManager->sizeX,
                                       windowManager->sizeY,
                                       winId, NULL);
  windowManager->wndCount = 1;
}

void destroyWindowManager(WindowManager *windowManager)
{
  while (windowManager->wndCount != 1) {
    windowManagerRemoveCurrentWindow(windowManager);
  }
  destroyWindow(windowManager->window);
  free(windowManager->window);
  windowManager->window = NULL;
}

Window *windowManagerGetWindow(WindowManager *windowManager)
{
  return windowManager->window;
}

Window *windowManagerGetWindowById(WindowManager *windowManager, unsigned id)
{
  Window *lastWin, *currWin;
  lastWin = windowManager->window->prev;
  currWin = windowManager->window;
  while (windowGetId(currWin) != id && currWin != lastWin) {
    currWin = currWin->next;
  }
  return (windowGetId(currWin) == id) ? currWin : NULL;
}

Window *windowManagerFindWindow(WindowManager *windowManager, char *bufName)
{
  Window *lastWin, *currWin;

  currWin = windowManager->window;
  lastWin = currWin;
  do {
    if (!strcmp(windowGetBufferName(currWin), bufName)) {
      return currWin;
    }
    currWin = currWin->next;
  } while (currWin != lastWin);
  return NULL;
}

void windowManagerPrevWindow(WindowManager *windowManager)
{
  windowManager->window = windowManager->window->prev;
}

void windowManagerNextWindow(WindowManager *windowManager)
{
  windowManager->window = windowManager->window->next;
}

unsigned
windowManagerSplitWindow(WindowManager *windowManager, Orientation orientation)
{
  Window *wnd;
  unsigned odd, newSizeX, newSizeY;
  unsigned newPosX, newPosY;
  unsigned currWndSizeX, currWndSizeY;
  unsigned currWndPosX, currWndPosY;
  char *bufName;
  unsigned newWndId;
  Mode *mode;

  windowGetSize(windowManager->window, &currWndSizeX, &currWndSizeY);
  windowGetPosition(windowManager->window, &currWndPosX, &currWndPosY);

  if (orientation == HORIZONTAL) {
    odd = currWndSizeY & 1;
    newSizeX = currWndSizeX;
    newSizeY = currWndSizeY >> 1;
    newPosX = currWndPosX;
    newPosY = currWndPosY + newSizeY;
    windowSetSize(windowManager->window, newSizeX, newSizeY);
    newSizeY += odd;
  }
  else {
    odd = currWndSizeX & 1;
    newSizeX = currWndSizeX >> 1;
    newSizeY = currWndSizeY;
    newPosX = currWndPosX + newSizeX;
    newPosY = currWndPosY;
    windowSetSize(windowManager->window, newSizeX, newSizeY);
    newSizeX += odd;
  }

  newWndId = generateWindowId_(windowManager);
  wnd = createWindow(newPosX, newPosY, newSizeX, newSizeY,
                     newWndId, NULL);
  windowManager->wndCount += 1;

  bufName = (char *)malloc(sizeof(char)
                           * strlen(windowGetBufferName(windowManager->window))
                           + 1);
  strcpy(bufName, windowGetBufferName(windowManager->window));
  mode = windowGetMode(windowManager->window);
  windowSetBufferName(wnd, bufName);
  windowSetMode(wnd, mode);

  wnd->prev = windowManager->window;
  wnd->next = windowManager->window->next;
  wnd->next->prev = wnd;
  windowManager->window->next = wnd;
  if (windowManager->window->prev == windowManager->window) {
    windowManager->window->prev = wnd;
  }

  return wnd->id;
}

void windowManagerRemoveCurrentWindow(WindowManager *windowManager)
{
  unsigned targetWndPosX, targetWndPosY, targetWndWidth, targetWndHeight;
  unsigned otherWndPosX, otherWndPosY, otherWndWidth, otherWndHeight;
  Window *targetWnd;
  Window *otherWnd;
  unsigned targetWndId;

  windowGetPosition(windowManager->window, &targetWndPosX, &targetWndPosY);
  windowGetSize(windowManager->window, &targetWndWidth, &targetWndHeight);
  targetWnd = windowManager->window;
  otherWnd = targetWnd->prev;

  while (otherWnd != targetWnd) {
    windowGetPosition(otherWnd, &otherWndPosX, &otherWndPosY);
    windowGetSize(otherWnd, &otherWndWidth, &otherWndHeight);

    /* The window to expand is above the target window */
    if ((otherWndPosX == targetWndPosX)
        && ((otherWndPosY + otherWndHeight) == targetWndPosY)) {
      unsigned newHeight = targetWndHeight + otherWndHeight;
      windowSetSize(otherWnd, otherWndWidth, newHeight);
      break;
    }

    /* The window to expand is below the target window */
    else if ((otherWndPosX == targetWndPosX)
        && ((otherWndPosY - targetWndHeight) == targetWndPosY)) {
      unsigned newHeight = otherWndHeight + targetWndHeight;
      windowSetPosition(otherWnd, otherWndPosX, targetWndPosY);
      windowSetSize(otherWnd, otherWndWidth, newHeight);
      break;
    }

    /* The window to expand is on the left side of the target window */
    else if ((otherWndPosY == targetWndPosY)
        && ((otherWndPosX + otherWndWidth) == targetWndPosX)) {
      unsigned newWidth = targetWndWidth + otherWndWidth;
      windowSetSize(otherWnd, newWidth, otherWndHeight);
      break;
    }

    /* The window to expand is on the right side of the target window */
    else if ((otherWndPosY == targetWndPosY)
        && ((otherWndPosX - targetWndWidth) == targetWndPosX)) {
      unsigned newWidth = otherWndWidth + targetWndWidth;
      windowSetPosition(otherWnd, targetWndPosX, otherWndPosY);
      windowSetSize(otherWnd, newWidth, otherWndHeight);
      break;
    }

    /* Still didn't find another window to occupy the target window's space,
       continue looping. */
    otherWnd = otherWnd->prev;
  }

  if (otherWnd == targetWnd) {
    /* TODO: Error, didn't find another window to expand. */
    assert(windowManager->wndCount <= 1);
    return;
  }

  targetWnd->prev->next = targetWnd->next;
  targetWnd->next->prev = targetWnd->prev;
  windowManager->window = otherWnd;
  targetWndId = windowGetId(targetWnd);
  destroyWindow(targetWnd);
  free(targetWnd);
  windowManager->wndCount--;
  freeWindowId_(windowManager, targetWndId);
}

void windowManagerResizeWindow(WindowManager *windowManager, unsigned height)
{
  /* TODO */
}
