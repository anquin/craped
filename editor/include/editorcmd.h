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

#include "editor.h"

typedef struct editor_cmd_declaration EditorCmdDeclaration;
typedef struct editor_cmd EditorCmd;

typedef void (*EditorCmdExecuteFn)(Editor *, EditorCmd *);
typedef void (*EditorExtensionCmdExecuteFn)(void *, void *);

struct editor_cmd_declaration
{
  char *name;
  EditorCmdExecuteFn execFn;
  char *editorExtName;
  EditorExtensionCmdExecuteFn customFn;
};

EditorCmdDeclaration *
createEditorCmdDeclaration(char *name, EditorCmdExecuteFn fn);
EditorCmdDeclaration *
createEditorExtensionCmdDeclaration(const char *editorExtName,
                                    char *name, EditorExtensionCmdExecuteFn fn);

struct editor_cmd
{
  int paramSz;
  char *param;
  EditorCmdDeclaration *decl;
};

void editorCmdExecute(EditorCmd *, Editor *);


typedef struct editor_cmd_home EditorCmdHome;

EditorCmdHome *createEditorCmdHome(void);
void destroyEditorCmdHome(EditorCmdHome *);
void editorCmdHomeRegister(EditorCmdHome *, EditorCmdDeclaration *cmdDecl);
void editorCmdHomeUnregister(EditorCmdHome *, char *);
EditorCmd *editorCmdHomeCreateCmd(EditorCmdHome *, char *name, int paramSz, char *param);

#endif
