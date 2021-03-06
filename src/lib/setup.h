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

#ifndef SETUP_H
#define SETUP_H

#include <uicore/ui.h>

typedef struct ui_setup UISetup;

typedef struct editor_setup
{
  const char *progName;
  UI *ui;
  struct ui_setup *uiSetup;

  int nFilesToOpen;
  char **filesToOpen;
} EditorSetup;

void setup(EditorSetup *edsetup, int argc, char **argv, const char *welcomeMsg);
void cleanup(EditorSetup *edsetup);

#endif
