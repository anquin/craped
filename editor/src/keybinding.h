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

#ifndef KEYBINDING_H
#define KEYBINDING_H

#include <uicore/keys.h>

typedef struct editor_cmd_tree_node EditorCmdTreeNode;
typedef EditorCmdTreeNode EditorCmdTree;

#include "editorcmd.h"
#include <libsys/hashing.h>

struct editor_cmd_tree_node
{
  EditorCmd *cmd;
  struct editor_cmd_tree_node *subTree[KEY_MAX_VAL];
  HashTable *symbolSubTree;
};


void initEditorCmdTree(EditorCmdTree *editorCmdTree);
/* Destroys all subtrees */
void destroyEditorCmdTree(EditorCmdTree *editorCmdTree);
/* kbInputList is always NULL terminated */
void
editorCmdTreeBind(EditorCmdTree *editorCmdTree,
                  KbInput *kbInputList, EditorCmd *editorCmd);
void editorCmdTreeUnbind(EditorCmdTree *editorCmdTree, KbInput *kbInputList);
void editorCmdTreeUnbindAll(EditorCmdTree *editorCmdTree);
EditorCmdTree *editorCmdTreeGet(EditorCmdTree *editorCmdTree, KbInput *kbInputList);
EditorCmd *editorCmdTreeGetCmd(EditorCmdTree *);

#endif
