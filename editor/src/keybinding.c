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

#include "keybinding.h"

#include <stdlib.h>
#include <string.h>

EditorCmdTree *createEditorCmdTree(void)
{
  EditorCmdTree *node;
  node = (EditorCmdTree *)malloc(sizeof(EditorCmdTree));
  initEditorCmdTree(node);
  return node;
}

void initEditorCmdTree(EditorCmdTree *node)
{
  node->cmd = NULL;
  memset(node->subTree, 0, KEY_MAX_VAL * sizeof(EditorCmdTree *));
  node->symbolSubTree = (HashTable *)malloc(sizeof(HashTable));
  initHashTable(node->symbolSubTree, 257);  /* 257 is prime */
}

void destroyEditorCmdTree(EditorCmdTree *node)
{
  int i;

  for (i = 0; i < KEY_MAX_VAL; i++) {
    if (node->subTree[i] != NULL) {
      destroyEditorCmdTree(node->subTree[i]);
      destroyHashTable(node->subTree[i]->symbolSubTree);
      free(node->subTree[i]);
      node->subTree[i] = NULL;
    }
  }
  destroyHashTable(node->symbolSubTree);
  free(node->symbolSubTree);
}


void editorCmdTreeBind(EditorCmdTree *node, KbInput *kbInputList, EditorCmd *cmd)
{
  if (kbInputList->key == NO_KEY) {
    node->cmd = cmd;
    return;
  }

  while ((kbInputList)->key != NO_KEY) {
    if (!kbInputList->size) {
      if (!(node->subTree[kbInputList->key])) {
        node->subTree[kbInputList->key] = createEditorCmdTree();
      }
      node = node->subTree[kbInputList->key];
    }
    else {
      KbInput *kbInputClone;
      Hasheable *hasheable;
      EditorCmdTree *newNode;
      kbInputClone = (KbInput *)malloc(sizeof(KbInput));
      *kbInputClone = *kbInputList;
      newNode = createEditorCmdTree();
      hasheable = createHasheable((unsigned (*)(void *, unsigned int))
                                  kbInputHashFn,
                                  (int (*)(void *, void *))kbInputEqualsFn,
                                  (void *)kbInputClone);
      hashTablePut(node->symbolSubTree, hasheable, (void *)newNode);
      node = newNode;
    }
    kbInputList = kbInputList + 1;
  }

  node->cmd = cmd;
}

void editorCmdTreeUnbind(EditorCmdTree *node, KbInput *kbInputList)
{
  if (kbInputList->key == NO_KEY) {
    node->cmd = NULL;
    return;
  }

  if (kbInputList->key != KEY_SYMBOL) {
    if (node->subTree != NULL) {
      editorCmdTreeUnbind(node->subTree[kbInputList->key], kbInputList + 1);
    }
    else {
      /* Given key sequence is not bound */
      return;
    }
  }
  else {
    Hasheable *hasheable;
    EditorCmdTree *nextNode;
    hasheable = createHasheable((unsigned (*)(void *, unsigned))kbInputHashFn,
                                (int (*)(void *, void *))kbInputEqualsFn,
                                (void *)kbInputList);
    nextNode = hashTableGet(node->symbolSubTree, hasheable);
    if (nextNode != NULL) {
      editorCmdTreeUnbind(nextNode, kbInputList + 1);
    }
  }
}

void editorCmdTreeUnbindAll(EditorCmdTree *node)
{
  int i;

  if (node == NULL) {
    return;
  }

  node->cmd = NULL;
  for (i = 0; i < KEY_MAX_VAL; i++) {
    editorCmdTreeUnbindAll(node->subTree[i]);
  }

  /* TODO: iterate over symbolSubTree to remove all entries */
}

EditorCmdTree *editorCmdTreeGet(EditorCmdTree *node, KbInput *kbInputList)
{
  unsigned i;
  EditorCmd *editorCmd;

  if (kbInputList->key == NO_KEY) {
    return node;
  }

  if (!(kbInputList->size)) {
    if (node->subTree[kbInputList->key] == NULL) {
      return NULL;
    }
    return editorCmdTreeGet(node->subTree[kbInputList->key], kbInputList + 1);
  }
  else {
    Hasheable *hasheable;
    EditorCmdTree *nextNode;
    hasheable = createHasheable((unsigned (*)(void *, unsigned))kbInputHashFn,
                                (int (*)(void *, void *))kbInputEqualsFn,
                                kbInputList);
    nextNode = hashTableGet(node->symbolSubTree, hasheable);
    if (nextNode != NULL) {
      return editorCmdTreeGet(nextNode, kbInputList + 1);
    }
    return NULL;
  }
}

EditorCmd *editorCmdTreeGetCmd(EditorCmdTree *editorCmdTree)
{
  return editorCmdTree->cmd;
}
