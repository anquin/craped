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

#ifndef DIFFEX_UI_H
#define DIFFEX_UI_H

#include <ui-core.h>
#include <subeditor.h>

typedef struct ui DiffexUI;

#include <diffexui/actual-ui.h>

#define MINI_WINDOW_ID WNDMAN_MAX_WINDOWS

#include <diffexui/diffex.h>

struct ui
{
  WindowManager *windowManager;
  TerminalFactory *termFactory;
  Terminal *terminals[WNDMAN_MAX_WINDOWS + 1];
  char *centeredMessage;
  unsigned centeredMessageSize;
  Diffex *diffex;
  struct editor *observer;
  ActualUI *actualUi;
  Window *miniWindow;
  Terminal *miniTerminal;
  short miniWindowActive;
  short forceCleanup;
};

void initDiffexUI(DiffexUI *, ActualUI *actualUi,
                  WindowManager *, TerminalFactory *,
                  Terminal *miniTerminal);
void destroyDiffexUI(DiffexUI *);

#endif
