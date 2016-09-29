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

#include <gtk-ui.h>
#include <gtkui/vtefactory.h>

#include <gtk/gtk.h>

struct ui_setup
{
  GtkUI *gtkUi;
  WindowManager *wndMan;
  TerminalFactory *termFactory;
};

void setupUI(EditorSetup *edsetup)
{
  UI *ui;
  WindowManager *wndMan;
  TerminalFactory *termFactory;
  VTE *miniTerm;

  termFactory = (VteTerminalFactory *)malloc(sizeof(TerminalFactory));
  initVteTerminalFactory(termFactory);
  wndMan = (WindowManager *)malloc(sizeof(WindowManager));
  initWindowManager(wndMan);

  GtkUI *gtkUi = (GtkUI *)malloc(sizeof(GtkUI));
  initGtkUI(gtkUi, edsetup->progName);

  ui = (DiffexUI *)malloc(sizeof(DiffexUI));
  miniTerm = createVte();
  vteSetSize(miniTerm, 80, 1);
  initDiffexUI(ui, gtkUi, wndMan, termFactory, miniTerm);

  edsetup->ui = ui;
  edsetup->uiSetup = (UISetup *)malloc(sizeof(UISetup));
  edsetup->uiSetup->gtkUi = gtkUi;
  edsetup->uiSetup->wndMan = wndMan;
  edsetup->uiSetup->termFactory = termFactory;
}

void cleanupUI(EditorSetup *edsetup)
{
  destroyDiffexUI(edsetup->ui);
  free(edsetup->ui);

  destroyGtkUI(edsetup->uiSetup->gtkUi);
  free(edsetup->uiSetup->gtkUi);

  destroyWindowManager(edsetup->uiSetup->wndMan);
  free(edsetup->uiSetup->wndMan);

  destroyTerminalFactory(edsetup->uiSetup->termFactory);
  free(edsetup->uiSetup->termFactory);

  free(edsetup->uiSetup);
}
