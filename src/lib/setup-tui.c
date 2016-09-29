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

#include "setup-ui.h"

#include <tui.h>
#include <tui/xterm.h>
#include <tui/xtermfactory.h>

struct ui_setup
{
  TextUI *textUi;
  XTerm *rootTerm;
  XTerm *uiRootTerm;
  WindowManager *wndMan;
  TerminalFactory *termFactory;
};

void setupUI(EditorSetup *edsetup)
{
  UI *ui;
  TextUI *textUi;
  WindowManager *wndMan;
  TerminalFactory *termFactory;
  XTerm *rootTerm;
  XTerm *uiRootTerm;
  XTerm *miniTerm;

  rootTerm = createXTerm(NULL, 0, 0);
  miniTerm = createXTerm(rootTerm, 1, terminalGetHeight(rootTerm) - 1);
  xtermSetHeight(miniTerm, 1);
  xtermSetWidth(miniTerm, terminalGetWidth(rootTerm) - 1);
  uiRootTerm = createXTerm(rootTerm, 0, 0);
  xtermSetHeight(uiRootTerm, terminalGetHeight(rootTerm) - 1);
  termFactory = (TerminalFactory *)malloc(sizeof(TerminalFactory));
  initXtermTerminalFactory(termFactory, uiRootTerm);

  wndMan = (WindowManager *)malloc(sizeof(WindowManager));
  initWindowManager(wndMan);

  textUi = (TextUI *)malloc(sizeof(TextUI));
  initTextUI(textUi, uiRootTerm);

  ui = (DiffexUI *)malloc(sizeof(DiffexUI));
  initDiffexUI(ui, textUi, wndMan, termFactory, miniTerm);

  edsetup->uiSetup = (UISetup *)malloc(sizeof(UISetup));
  edsetup->ui = ui;
  edsetup->uiSetup->textUi = textUi;
  edsetup->uiSetup->wndMan = wndMan;
  edsetup->uiSetup->termFactory = termFactory;
  edsetup->uiSetup->uiRootTerm = uiRootTerm;
  edsetup->uiSetup->rootTerm = rootTerm;
}

void cleanupUI(EditorSetup *edsetup)
{
  destroyDiffexUI(edsetup->ui);
  free(edsetup->ui);

  destroyTextUI(edsetup->uiSetup->textUi);
  free(edsetup->uiSetup->textUi);

  destroyWindowManager(edsetup->uiSetup->wndMan);
  free(edsetup->uiSetup->wndMan);

  destroyTerminalFactory(edsetup->uiSetup->termFactory);
  free(edsetup->uiSetup->termFactory);

  destroyTerminal(edsetup->uiSetup->uiRootTerm);
  free(edsetup->uiSetup->uiRootTerm);

  destroyTerminal(edsetup->uiSetup->rootTerm);
  free(edsetup->uiSetup->rootTerm);

  free(edsetup->uiSetup);
}
