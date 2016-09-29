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

#ifndef EDITORCMD_H
#define EDITORCMD_H

typedef struct editor_cmd EditorCmd;
typedef struct editor_cmd_home EditorCmdHome;

#include "editor.h"

typedef void (*EditorCmdExecuteFn)(Editor *, EditorCmd *);

EditorCmdHome *createEditorCmdHome(void);
void destroyEditorCmdHome(EditorCmdHome *);
void editorCmdHomeRegister(EditorCmdHome *, char *, EditorCmdExecuteFn);
void editorCmdHomeUnregister(EditorCmdHome *, char *);
EditorCmd *editorCmdHomeCreateCmd(EditorCmdHome *, char *name, int paramSz, char *param);

struct editor_cmd
{
  int paramSz;
  char *param;
  EditorCmdExecuteFn editorCmdExecuteFn;
};

void editorCmdExecute(EditorCmd *, Editor *);

#endif
