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

#include "config.h"

#include <libsys/dbglog.h>

#include <editor.h>
#include <uicore/buffer_point_moves.h>
#include "setup.h"
#include "setup-ui.h"

#include <stdlib.h>
#include <stdio.h>

static const char progname[] = PACKAGE_NAME;

int main(int argc, char *argv[])
{
  EditorSetup edsetup;
  Editor *editor;
  Mode *mode;

  DBGLOG_OPEN(fopen("/tmp/crapeddbg.log", "wb"));

  setup(&edsetup, argc, argv);

  mode = (Mode *)malloc(sizeof(Mode));
  initMode(mode, utf8MovePoint, utf8WordMovePoint, utf8LineMovePoint);
  windowSetMode(uiGetWindow(edsetup.ui), mode);

  editor = createEditor(edsetup.ui, PACKAGE_STRING);
  editorRun(editor);
  destroyEditor(editor);
  free(editor);
  cleanup(&edsetup);

  DBGLOG_CLOSE();

  return 0;
}

void setup(EditorSetup *edsetup, int argc, char *argv[])
{
  edsetup->progName = progname;
  /* TODO: parse configuration file */
  /* TODO: parse command line args */
  setupUI(edsetup);
}

void cleanup(EditorSetup *edsetup)
{
  cleanupUI(edsetup);
}
