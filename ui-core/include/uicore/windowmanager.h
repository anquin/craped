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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "window.h"

#define WNDMAN_MAX_SPLITS_H 10
#define WNDMAN_MAX_SPLITS_V 10
#define WNDMAN_MAX_WINDOWS WNDMAN_MAX_SPLITS_H * WNDMAN_MAX_SPLITS_V

typedef struct window_manager
{
  unsigned sizeX;
  unsigned sizeY;
  /* Window chain */
  Window *window;  /* TODO: use array. */
  unsigned wndCount;

  unsigned unusedIds[WNDMAN_MAX_WINDOWS];
} WindowManager;

void initWindowManager(WindowManager *);
void destroyWindowManager(WindowManager *);

Window *windowManagerGetWindow(WindowManager *);
Window *windowManagerGetWindowById(WindowManager *, unsigned id);
void windowManagerPrevWindow(WindowManager *);
void windowManagerNextWindow(WindowManager *);
typedef enum {HORIZONTAL, VERTICAL} Orientation;
/* Returns the id of the new window */
unsigned windowManagerSplitWindow(WindowManager *, Orientation);
void windowManagerRemoveCurrentWindow(WindowManager *);
void windowManagerResizeWindow(WindowManager *windowManager, unsigned height);
#define windowManagerGetWindowCount(wndMan) ((wndMan)->wndCount)

#endif
