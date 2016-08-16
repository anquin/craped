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

#include "testcommons.h"

#include <stdlib.h>
#include <assert.h>
#include "editor/editorcmd.h"
#include "editor/keybinding.h"

int main(void)
{
  EditorCmd *editorCmd;
  KbInput *kbInput;
  EditorCmdHome *editorCmdHome;

  editorCmdHome = (EditorCmdHome *)malloc(sizeof(EditorCmdHome));
  initEditorCmdHome(editorCmdHome);

  editorCmdHomeRegister(editorCmdHome, "exit", editorCmdFnExit);
  editorCmd = editorCmdHomeCreateCmd(editorCmdHome, "exit", 0, NULL);

  assert(editorCmd != NULL);
  assert(editorCmd->editorCmdExecuteFn == editorCmdFnExit);

  EditorCmdTree *editorCmdTree;
  initEditorCmdTree(editorCmdTree);

  kbInput = (KbInput *)malloc(sizeof(KbInput) * 2);
  kbInput[0].key = KEY_CTRL_SYMBOL;
  kbInput[0].info = "q";
  kbInput[0].size = 1;
  kbInput[1].key = NO_KEY;

  editorCmd = editorCmdHomeCreateCmd(editorCmdHome, "exit", 0, NULL);
  editorCmdTreeBind(editorCmdTree, kbInput, editorCmd);

  editorCmdTree = editorCmdTreeGet(editorCmdTree, kbInput);
  assert(editorCmdTree != NULL);
  assert(editorCmdTreeGetCmd(editorCmdTree) != NULL);
  assert(editorCmdTreeGetCmd(editorCmdTree)->editorCmdExecuteFn
         == editorCmd->editorCmdExecuteFn);
  assert(editorCmd->editorCmdExecuteFn == editorCmdFnExit);

  return 0;
}
