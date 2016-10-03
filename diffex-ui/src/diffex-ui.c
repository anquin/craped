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

#include "diffex-ui.h"
#include <uicore/framer.h>
#include <uicore/buffer_point_moves.h>
#include <uicore/terminal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initDiffexUI(DiffexUI *diffexUi, ActualUI *actualUi,
                  WindowManager *windowManager, TerminalFactory *termFactory,
                  Terminal *miniTerminal)
{
  int i;
  Mode *mode;
  diffexUi->actualUi = actualUi;
  diffexUi->windowManager = windowManager;
  diffexUi->termFactory = termFactory;
  mode = (Mode *)malloc(sizeof(Mode));
  initMode(mode, utf8MovePoint, utf8WordMovePoint, utf8LineMovePoint);
  diffexUi->miniWindow = createWindow(0, 0, 0, 0, MINI_WINDOW_ID, mode);
  diffexUi->miniTerminal = miniTerminal;
  diffexUi->miniWindowActive = 0;
  windowSetHasStatusLine(diffexUi->miniWindow, 0);
  windowSetBufferName(diffexUi->miniWindow, "*messages*");
  for (i = 1; i <= WNDMAN_MAX_WINDOWS; i++) {
    diffexUi->terminals[i] = NULL;
  }
  diffexUi->terminals[windowGetId(windowManagerGetWindow(windowManager))]
    = terminalFactoryCreate(termFactory);
  diffexUi->diffex = (Diffex *)malloc(sizeof(Diffex));
  initDiffex(diffexUi->diffex, 4096 << 1);
  diffexSetMode(diffexUi->diffex, DE_SHOW);
}

void destroyDiffexUI(DiffexUI *diffexUi)
{
  int i;
  diffexUi->windowManager = NULL;
  diffexUi->termFactory = NULL;
  for (i = 0; i <= WNDMAN_MAX_WINDOWS; i++) {
    if (diffexUi->terminals[i] != NULL) {
      destroyTerminal(diffexUi->terminals[i]);
      free(diffexUi->terminals[i]);
      diffexUi->terminals[i] = NULL;
    }
  }
  destroyDiffex(diffexUi->diffex);
}

void observerUpdate(DiffexUI *diffexUi, World *world)
{
  if (!worldIsAlive(world)) {
    diffexSetMode(diffexUi->diffex, DE_ERASE);
  }
  diffexView(diffexUi->diffex, diffexUi, world);
  if (diffexGetMode(diffexUi->diffex) == DE_SHOW) {
    diffexSetMode(diffexUi->diffex, DE_UPDATE);
  }
}

void uiRedisplay(UI *diffexUi, World *world)
{
  observerUpdate(diffexUi, world);
}

struct window *uiGetActiveWindow(UI *ui)
{
  if (ui->miniWindowActive) {
    return ui->miniWindow;
  }
  return windowManagerGetWindow(ui->windowManager);
}

struct window *uiFindWindow(UI *ui, char *bufName)
{
  return windowManagerFindWindow(ui->windowManager, bufName);
}

void uiSayCentered(UI *ui, const char *message)
{
  size_t i;

  ui->centeredMessageSize = strlen(message);
  if (!ui->centeredMessageSize) {
    return;
  }
  ui->centeredMessage =
    (char *)malloc(sizeof(char) * ui->centeredMessageSize + 1);

  for (i = 0; i < ui->centeredMessageSize; i++) {
    ui->centeredMessage[i] = message[i];
  }
  ui->centeredMessage[i] = '\0';
}

void uiMainLoop(UI *ui)
{
  ActualUIMainLoop(ui->actualUi, ui);
}

void uiSetObserver(UI *ui, struct editor *editor)
{
  ui->observer = editor;
}

void uiNextWindow(UI *ui)
{
  windowManagerNextWindow(ui->windowManager);

  /* Window *window; */

  /* if (ui->miniTerminalActive) { */
  /*   ui->miniTerminalActive = 0; */
  /* } */
  /* else { */
  /*   windowManagerNextWindow(ui->windowManager); */
  /*   window = windowManagerWindow(ui->windowManager); */
  /*   if (window == ui->firstWindow) { */
  /*     ui->miniTerminalActive = 1; */
  /*   } */
  /* } */
}

void uiPrevWindow(UI *ui)
{
  windowManagerPrevWindow(ui->windowManager);

  /* Window *window; */

  /* window = windowManagerWindow(ui->windowManager); */
  /* if (ui->miniTerminalActive) { */
  /*   ui->miniTerminalActive = 0; */
  /* } */
  /* else if (window == ui->firstWindow) { */
  /*   ui->miniTerminalActive = 1; */
  /* } */
  /* else { */
  /*   windowManagerPrevWindow(ui->windowManager); */
  /* } */
}

void uiRemoveCurrentWindow(UI *ui)
{
  unsigned winId;
  winId = windowGetId(uiGetActiveWindow(ui));
  destroyTerminal(ui->terminals[winId]);
  free(ui->terminals[winId]);
  ui->terminals[winId] = NULL;
  windowManagerRemoveCurrentWindow(ui->windowManager);
}

void uiSplitWindowHorz(UI *ui)
{
  unsigned wndId;
  if (ui->miniWindowActive) {
    return;
  }
  wndId = windowManagerSplitWindow(ui->windowManager, HORIZONTAL);
  ui->terminals[wndId] = terminalFactoryCreate(ui->termFactory);
}

void uiSetWindowBufferName(UI *ui, Window *wnd, char *bufName)
{
  windowSetBufferName(wnd, bufName);
}

char *uiGetWindowBufferName(UI *ui, Window *wnd)
{
  return windowGetBufferName(wnd);
}

/* void uiSetWindowHeight(UI *ui, unsigned height) */
/* { */
/*   windowManagerResizeWindow(ui->windowManager, height); */
/* } */

void uiSetWindowHasStatusLine(UI *ui, short flag)
{
  struct window *wnd;
  wnd = windowManagerGetWindow(ui->windowManager);
  windowSetHasStatusLine(wnd, flag);
}

/* short uiGetWindowHasStatusLine(UI *ui) */
/* { */
/*   return windowHasStatusLine(windowManagerGetWindow(ui->windowManager)) ? 1 : 0; */
/* } */

KbInput *uiWaitForInput(UI *ui)
{
}

void uiActivateMiniWindow(UI *ui, short flag)
{
  ui->miniWindowActive = flag;
}
