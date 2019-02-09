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

#include "tui.h"
#include <diffex-ui.h>
#include <diffexui/diffex-ui-private.h>
#include <tui/xterm.h>
#include <tui/xtermfactory.h>
#include <string.h>

#include <libsys/dbglog.h>

void initTextUI(TextUI *textUi)
{
  DiffexUI *ui;
  WindowManager *wndMan;
  TerminalFactory *termFactory;
  XTerm *rootTerm;
  XTerm *wndManRootTerm;
  XTerm *miniTerm;

  rootTerm = createXTerm(NULL, 0, 0);
  miniTerm = createXTerm(rootTerm, 1, terminalGetHeight(rootTerm) - 1);
  xtermSetHeight(miniTerm, 1);
  xtermSetWidth(miniTerm, terminalGetWidth(rootTerm) - 1);
  wndManRootTerm = createXTerm(rootTerm, 0, 0);
  xtermSetHeight(wndManRootTerm, terminalGetHeight(rootTerm) - 1);
  termFactory = (TerminalFactory *)malloc(sizeof(TerminalFactory));
  initXtermTerminalFactory(termFactory, wndManRootTerm);

  wndMan = (WindowManager *)malloc(sizeof(WindowManager));
  initWindowManager(wndMan);

  ui = (DiffexUI *)malloc(sizeof(DiffexUI));
  initDiffexUI(ui, textUi, wndMan, termFactory, miniTerm);

  textUi->ui = ui;
  textUi->wndMan = wndMan;
  textUi->rootTerm = rootTerm;
  textUi->wndManRootTerm = wndManRootTerm;
  textUi->termFactory = termFactory;
  textUi->miniTerm = miniTerm;
}

void destroyTextUI(TextUI *textUi)
{
  destroyWindowManager(textUi->wndMan);
  free(textUi->wndMan);

  destroyTerminalFactory(textUi->termFactory);
  free(textUi->termFactory);

  destroyTerminal(textUi->wndManRootTerm);
  free(textUi->wndManRootTerm);

  destroyTerminal(textUi->rootTerm);
  free(textUi->rootTerm);
}

void ActualUIPrepareTerminal(ActualUI *actualUi, DiffexUI *ui,
                             unsigned posX, unsigned posY,
                             unsigned sizeX, unsigned sizeY,
                             Terminal *terminal)
{
  XTerm *root;
  unsigned height, width;

  root = actualUi->rootTerm;

  if (terminal == actualUi->miniTerm) {
    xtermSetPosX(terminal, 1);
    xtermSetPosY(terminal, terminalGetHeight(root) - 1);
    return;
  }

  width = terminalGetWidth(root);
  height = terminalGetHeight(root) - terminalGetHeight(actualUi->miniTerm);
  xtermSetPosX(terminal, posX * width / WNDMAN_MAX_SPLITS_H + 1);
  xtermSetPosY(terminal, posY * height / WNDMAN_MAX_SPLITS_V);
  terminal->height = sizeY * height / WNDMAN_MAX_SPLITS_V - 1;
  terminal->width =  sizeX * width / WNDMAN_MAX_SPLITS_H - 1;
}

void ActualUIDrawTerminal(ActualUI *actualUi, DiffexUI *ui, Terminal *terminal)
{
  /* Nothing to be done yet */
}

void ActualUIEraseTerminal(ActualUI *actualUi, DiffexUI *ui, Terminal *terminal)
{
  terminalClear(terminal);
}


#define MAKE_BOX do {                                                   \
    box.x = xtermGetPosX(terminal) - 1;                                 \
    box.y = xtermGetPosY(terminal);                                     \
    box.h = terminalGetHeight(terminal);                                \
    box.w = terminalGetWidth(terminal);                                 \
    box.t = 0;                                                          \
    box.r = 0;                                                          \
    box.b = 0;                                                          \
    box.l = 1;                                                          \
  }while(0)

/* FIXME: Use proper getters/setters */
void ActualUIDrawBox(ActualUI *actualUi,
                     DiffexUI *diffexUi,
                     Terminal *terminal)
{
  struct box box;
  MAKE_BOX;
  DBGLOG("drawBox(%d, %d, %d, %d)\n",
         box.x, box.y, box.w, box.h);
  terminalDrawBox(actualUi->rootTerm, &box);
}

void ActualUIEraseBox(ActualUI *actualUi,
                      DiffexUI *diffexUi,
                      Terminal *terminal)
{
  struct box box;
  MAKE_BOX;
  terminalEraseBox(actualUi->rootTerm, &box);
}


#define WRITE_CENTERED(m, s) if (s) {                         \
    register unsigned halfSz;                                 \
    unsigned x, y;                                            \
    XTerm *mainTerm = actualUi->wndManRootTerm;               \
                                                              \
    halfSz = sz >> 1;                                         \
                                                              \
    x = (terminalGetWidth(mainTerm) >> 1) - halfSz;           \
    y = terminalGetHeight(mainTerm) >> 1;                     \
                                                              \
    terminalSetCursor(mainTerm, x, y);                        \
    terminalWrite(mainTerm, m, s);                            \
  }

void ActualUIDrawCenteredText(ActualUI *actualUi, DiffexUI *diffexUi,
                              char *text, unsigned sz)
{
  WRITE_CENTERED(text, sz);
}

void ActualUIEraseCenteredText(ActualUI *actualUi, DiffexUI *diffexUi,
                               char *text, unsigned sz)
{
  char *m;
  m = (char *)malloc(sizeof(char) * sz);
  memset(m, ' ', sz);
  WRITE_CENTERED(m, sz);
}

unsigned ActualUIGetCenteredTextMax(ActualUI *actualUi, DiffexUI *diffexUi)
{
  return terminalGetWidth(actualUi->rootTerm);
}

#define WRITE_STATUS_LINE(m) do {                                        \
    XTerm *term;                                                         \
    term = createXTerm(actualUi->wndManRootTerm, wndPosX, wndPosY);            \
    ActualUIPrepareTerminal(actualUi, ui, wndPosX, wndPosY,              \
                            wndSizeX, wndSizeY, term);                   \
    terminalSetCursor(actualUi->wndManRootTerm, xtermGetPosX(term),            \
                      xtermGetPosY(term) + terminalGetHeight(term));     \
    terminalWrite(actualUi->wndManRootTerm, m, size);                          \
    destroyTerminal(term);                                               \
    free(term);                                                          \
  }while(0)

void ActualUIDrawStatusLine(ActualUI *actualUi, DiffexUI *ui,
                            unsigned wndPosX, unsigned wndPosY,
                            unsigned wndSizeX, unsigned wndSizeY,
                            char *text, unsigned size)
{
  WRITE_STATUS_LINE(text);
}

void ActualUIEraseStatusLine(ActualUI *actualUi, DiffexUI *ui,
                             unsigned wndPosX, unsigned wndPosY,
                             unsigned wndSizeX, unsigned wndSizeY,
                             char *text, unsigned size)
{
  char *m;
  m = (char *)malloc(sizeof(char) * size);
  memset(m, ' ', size);
  WRITE_STATUS_LINE(m);
}

void ActualUIMainLoop(ActualUI *actualUi, UI *ui)
{
  KbInput *input;
  unsigned wndId;
  unsigned currRootTermWidth;
  unsigned currRootTermHeight;

  actualUi->prevRootTermWidth = terminalGetWidth(actualUi->rootTerm);
  actualUi->prevRootTermHeight = terminalGetHeight(actualUi->rootTerm);

  do {
    input = terminalGetInput(ui->terminals[windowGetId(uiGetActiveWindow(ui))]);
    terminalGetDimensions(actualUi->rootTerm,
                          &currRootTermWidth, &currRootTermHeight);
    if (currRootTermWidth != actualUi->prevRootTermWidth
        || currRootTermHeight != actualUi->prevRootTermHeight) {
      uiForceCleanupOnNextRedisplay(ui);
      actualUi->prevRootTermWidth = currRootTermWidth;
      actualUi->prevRootTermHeight = currRootTermHeight;
    }
  } while (uiObserverHandleInput(ui->observer, input) == UI_CONTINUE);
}
