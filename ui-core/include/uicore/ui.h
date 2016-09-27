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

#ifndef UI_H
#define UI_H

#include "ui_observer.h"
#include <subeditor.h>
#include "keys.h"
#include "window.h"

typedef struct world_observer UI;

void uiRedisplay(UI *, World *);
struct window *uiGetWindow(UI *);
void uiSayCentered(UI *, const char *);
void uiMainLoop(UI *);
void uiSetObserver(UI *, UIObserver *);
void uiNextWindow(UI *);
void uiPrevWindow(UI *);
void uiRemoveCurrentWindow(UI *);
void uiSplitWindowHorz(UI *);
void uiSetWindowBuffer(UI *, char *bufName);
char *uiGetWindowBufferName(UI *);
void uiSetWindowHeight(UI *, unsigned height);
void uiSetWindowHasStatusLine(UI *, short flag);
short uiGetWindowHasStatusLine(UI *);
KbInput *uiWaitForInput(UI *);
void uiActivateMiniWindow(UI *, short flag);

#endif
