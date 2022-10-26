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
#include <libsys.h>
#include "setup.h"
#include <diffex-ui.h>

struct ui_setup
{
  TextUI *textUi;
};

void setupUI(EditorSetup *edsetup)
{
  TextUI *textUi;

  textUi = (TextUI *)lsmalloc(sizeof(TextUI));
  initTextUI(textUi);

  edsetup->ui = textUi->ui;
  edsetup->uiSetup = (UISetup *)lsmalloc(sizeof(UISetup));
  edsetup->uiSetup->textUi = textUi;
}

void cleanupUI(EditorSetup *edsetup)
{
  destroyTextUI(edsetup->uiSetup->textUi);
  lsfree(edsetup->uiSetup->textUi);
  lsfree(edsetup->uiSetup);
  destroyDiffexUI(edsetup->ui);
  lsfree(edsetup->ui);
}
