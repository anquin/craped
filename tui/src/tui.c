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
#include <diffexui/diffex-ui-private.h>
#include <tui/xterm.h>
#include <string.h>

#include <libsys/dbglog.h>

void initTextUI(TextUI *textUi, Terminal *rootTerm)
{
  textUi->rootTerm = rootTerm;
}

void destroyTextUI(TextUI *actualUi)
{
}

void ActualUIPrepareTerminal(ActualUI *actualUi, DiffexUI *ui,
                             unsigned posX, unsigned posY,
                             unsigned sizeX, unsigned sizeY,
                             Terminal *terminal)
{
  XTerm *root;

  if (terminal == uiGetMiniTerminal_(ui)) {
    return;
  }

  root = actualUi->rootTerm;
  terminal->posX = posX * terminalGetWidth(root) / 10 + 1;
  terminal->posY = posY * terminalGetHeight(root) / 10;
  terminal->height = sizeY * terminalGetHeight(root) / 10 - 1;
  terminal->width =  sizeX * terminalGetWidth(root) / 10 - 1;
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
    XTerm *root = actualUi->rootTerm;                         \
                                                              \
    halfSz = sz >> 1;                                         \
                                                              \
    x = (terminalGetWidth(root) >> 1) - halfSz;               \
    y = terminalGetHeight(root) >> 1;                         \
                                                              \
    terminalSetCursor(root, x, y);                            \
    terminalWrite(root, m, s);                                \
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
    term = createXTerm(actualUi->rootTerm, wndPosX, wndPosY);            \
    ActualUIPrepareTerminal(actualUi, ui, wndPosX, wndPosY,              \
                            wndSizeX, wndSizeY, term);                   \
    terminalSetCursor(actualUi->rootTerm, xtermGetPosX(term),            \
                      xtermGetPosY(term) + terminalGetHeight(term));     \
    terminalWrite(actualUi->rootTerm, m, size);                       \
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

  do {
    input = terminalGetInput(ui->terminals[windowGetId(uiGetWindow(ui))]);
  } while (uiObserverHandleInput(ui->observer, input) == UI_CONTINUE);
}
