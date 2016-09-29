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

#include "editorcmd.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <libsys/hashing.h>

EditorCmdDeclaration *
createEditorCmdDeclaration(char *name, EditorCmdExecuteFn fn)
{
  EditorCmdDeclaration *decl;
  decl = (EditorCmdDeclaration *)malloc(sizeof(EditorCmdDeclaration));
  decl->name = strcpy((char *)malloc(sizeof(char) * (strlen(name) + 1)), name);
  decl->execFn = fn;
  decl->editorExtName = NULL;
  decl->customFn = NULL;
  return decl;
}

EditorCmdDeclaration *
createEditorExtensionCmdDeclaration(const char *editorExtName,
                                    char *name, EditorExtensionCmdExecuteFn fn)
{
  EditorCmdDeclaration *decl;
  decl = (EditorCmdDeclaration *)malloc(sizeof(EditorCmdDeclaration));
  decl->name = strcpy((char *)malloc(sizeof(char) * (strlen(name) + 1)), name);
  decl->execFn = NULL;
  decl->editorExtName =
    strcpy((char *)malloc(sizeof(char) * (strlen(name) + 1)), editorExtName);
  decl->customFn = fn;
  return decl;
}

struct editor_cmd_home
{
  HashTable editorCmdEntries;
};

void editorCmdExecute(EditorCmd *editorCmd, Editor *editor)
{
  if (editorCmd->decl->execFn != NULL) {
    return editorCmd->decl->execFn(editor, editorCmd);
  }
  /* FIXME: These asserts... */
  assert(editorCmd->decl != NULL);
  EditorExtension *editorExt;
  editorExt =
    editorGetExtension(editor, editorCmd->decl->editorExtName);
  assert(editorExt != NULL);
  return editorCmd->decl->customFn(editorExtensionGetImpl(editorExt),
                                   (void *)editorCmd);
}

EditorCmdHome *createEditorCmdHome(void)
{
  EditorCmdHome *editorCmdHome;
  editorCmdHome = (EditorCmdHome *)malloc(sizeof(EditorCmdHome));
  initHashTable(&editorCmdHome->editorCmdEntries, 227);
  return editorCmdHome;
}

void destroyEditorCmdHome(EditorCmdHome *editorCmdHome)
{
  destroyHashTable(&editorCmdHome->editorCmdEntries);
}

void editorCmdHomeRegister(EditorCmdHome *editorCmdHome,
                           EditorCmdDeclaration *cmdDecl)
{
  Hasheable *hasheable;
  hasheable = createHasheable(strHashFn,
                              (int (*)(void *, void *))strcmp,
                              (void *)cmdDecl->name);
  hashTablePut(&editorCmdHome->editorCmdEntries, hasheable, cmdDecl);
}

void editorCmdHomeUnregister(EditorCmdHome *editorCmdHome, char *name)
{
  Hasheable *hasheable;
  hasheable = createHasheable(strHashFn,
                              (int (*)(void *, void *))strcmp,
                              (void *)name);
  hashTableRemove(&editorCmdHome->editorCmdEntries, hasheable);
}

EditorCmd *
editorCmdHomeCreateCmd(EditorCmdHome *editorCmdHome, char *name, int paramSz, char *param)
{
  EditorCmd *editorCmd;
  Hasheable *hasheable;
  hasheable = createHasheable(strHashFn,
                              (int (*)(void *, void *))strcmp,
                              (void *)name);

  editorCmd = (EditorCmd *)malloc(sizeof(EditorCmd));
  editorCmd->paramSz = paramSz;
  editorCmd->param = param;
  editorCmd->decl =
    (EditorCmdDeclaration *)hashTableGet(&editorCmdHome->editorCmdEntries,
                                         hasheable);

  if (editorCmd->decl == NULL) {
    return NULL;
  }

  return editorCmd;
}
