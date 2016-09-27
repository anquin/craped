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
#include "string.h"

#include <stdio.h>
#include <stdlib.h>

//#include "dbg/dbglog.h"

char *parseStringParam(EditorCmd *editorCmd)
{
  char *s;
  s = (char *)malloc(sizeof(char) * editorCmd->paramSz + 1);
  strncpy(s, editorCmd->param, editorCmd->paramSz);
  s[editorCmd->paramSz] = '\0';

  return s;
}

/* Parses an editorCmd param as an integer. Returns zero if fails. */
int parseIntParam(EditorCmd *editorCmd, int *param)
{
  int i;

  if (!editorCmd->paramSz) {
    return 0;
  }

  *param = 0;

  for (i = 0; i < editorCmd->paramSz; i++) {
    if (editorCmd->param[i] < '0' || editorCmd->param[i] > '9') {
      return 0;
    }
    (*param) = (*param) * 10 + editorCmd->param[i] - '0';
  }

  return 1;
}

void editorCmdExecute(EditorCmd *editorCmd, Editor *editor)
{
  return editorCmd->editorCmdExecuteFn(editor, editorCmd);
}

void initEditorCmdHome(EditorCmdHome *editorCmdHome)
{
  initHashTable(&editorCmdHome->editorCmdEntries, 227);
}

void destroyEditorCmdHome(EditorCmdHome *editorCmdHome)
{
  destroyHashTable(&editorCmdHome->editorCmdEntries);
}

void
editorCmdHomeRegister(EditorCmdHome *editorCmdHome, char *name, EditorCmdExecuteFn execFn)
{
  Hasheable *hasheable;
  hasheable = createHasheable(strHashFn,
                              (int (*)(void *, void *))strcmp,
                              (void *)name);
  hashTablePut(&editorCmdHome->editorCmdEntries, hasheable, execFn);
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
  editorCmd->editorCmdExecuteFn =
    (EditorCmdExecuteFn)hashTableGet(&editorCmdHome->editorCmdEntries, hasheable);

  if (editorCmd->editorCmdExecuteFn == NULL) {
    return NULL;
  }

  return editorCmd;
}


void editorCmdFnExit(Editor *editor, EditorCmd *editorCmd)
{
  editorExit(editor);
}

void editorCmdFnInsert(Editor *editor, EditorCmd *editorCmd)
{
  editorInsert(editor, editorCmd->param, editorCmd->paramSz);
}

void editorCmdFnBackspace(Editor *editor, EditorCmd *editorCmd)
{
  editorBackspace(editor);
}

void editorCmdFnDelete(Editor *editor, EditorCmd *editorCmd)
{
  editorDelete(editor);
}

void editorCmdFnLineFeed(Editor *editor, EditorCmd *editorCmd)
{
  editorLineFeed(editor);
}

void editorCmdFnInsertTab(Editor *editor, EditorCmd *editorCmd)
{
  editorInsertTab(editor);
}

void editorCmdFnMoveLeft(Editor *editor, EditorCmd *editorCmd)
{
  editorMoveLeft(editor);
}

void editorCmdFnMoveRight(Editor *editor, EditorCmd *editorCmd)
{
  editorMoveRight(editor);
}

void editorCmdFnMoveUp(Editor *editor, EditorCmd *editorCmd)
{
  editorMoveUp(editor);
}

void editorCmdFnMoveDown(Editor *editor, EditorCmd *editorCmd)
{
  editorMoveDown(editor);
}

void editorCmdFnPrevWord(Editor *editor, EditorCmd *editorCmd)
{
  editorPrevWord(editor);
}

void editorCmdFnNextWord(Editor *editor, EditorCmd *editorCmd)
{
  editorNextWord(editor);
}

void editorCmdFnPromptToConnectToSharing(Editor *editor, EditorCmd *editorCmd)
{
  char *port;
  port = editorRecoverFromPromptedInput(editor);
  editorCmd->param = port;
  editorCmd->paramSz = strlen(port);
  editorCmdFnConnectToSharing(editor, editorCmd);
}

void editorCmdFnConnectToSharing(Editor *editor, EditorCmd *editorCmd)
{
  int port;
  if (editorCmd->paramSz) {
    if (!(parseIntParam(editorCmd, &port))) {
      editorShowMessage(editor, "Invalid port.");
    }
    else {
      editorConnectToSharing(editor, "127.0.0.1", port);
    }
  }
  else {
    editorPromptForInput(editor, "prompt_to_sharing_connect",
                      "Enter a port number...");
  }
}

void editorCmdFnChooseBuffer(Editor *editor, EditorCmd *editorCmd)
{
  char *bufferName;
  bufferName = parseStringParam(editorCmd);
  editorChooseBuffer(editor, bufferName);
  free(bufferName);
}

void editorCmdFnPromptToSetWindowBuffer(Editor *editor, EditorCmd *editorCmd)
{
  char *bufferName;
  bufferName = editorRecoverFromPromptedInput(editor);
  editorChooseWindowBuffer(editor, bufferName);
}

void editorCmdFnChooseWindowBuffer(Editor *editor, EditorCmd *editorCmd)
{
  char *bufferName;
  if (editorCmd->paramSz) {
    bufferName = parseStringParam(editorCmd);
    editorChooseWindowBuffer(editor, bufferName);
    free(bufferName);
  }
  else {
    editorPromptForInput(editor,
                         "prompt_to_set_window_buffer",
                         "Enter a buffer name...");
  }
}

void editorCmdFnSplitWindowHorz(Editor *editor, EditorCmd *editorCmd)
{
  editorSplitWindowHorz(editor);
}

void editorCmdFnRemoveCurrentWindow(Editor *editor, EditorCmd *editorCmd)
{
  editorRemoveCurrentWindow(editor);
}

void editorCmdFnNextWindow(Editor *editor, EditorCmd *editorCmd)
{
  editorNextWindow(editor);
}

void editorCmdFnPrevWindow(Editor *editor, EditorCmd *editorCmd)
{
  editorPrevWindow(editor);
}

void editorCmdFnPromptToInitSharing(Editor *editor, EditorCmd *editorCmd)
{
  char *port;
  port = editorRecoverFromPromptedInput(editor);
  editorCmd->param = port;
  editorCmd->paramSz = strlen(port);
  editorCmdFnInitSharing(editor, editorCmd);
}

void editorCmdFnInitSharing(Editor *editor, EditorCmd *editorCmd)
{
  int port;
  if (editorCmd->paramSz) {
    if (!(parseIntParam(editorCmd, &port))) {
      editorShowMessage(editor, "Invalid port.");
    }
    else {
      editorSetSharingPort(editor, port);
      editorInitSharing(editor);
    }
  }
  else {
    editorPromptForInput(editor, "prompt_to_init_sharing", "Enter a port number...");
  }
}

void editorCmdFnNextBuffer(Editor *editor, EditorCmd *editorCmd)
{
  editorNextBuffer(editor);
}

void editorCmdFnPrevBuffer(Editor *editor, EditorCmd *editorCmd)
{
  editorPrevBuffer(editor);
}

void editorCmdFnCloseCurrentBuffer(Editor *editor, EditorCmd *editorCmd)
{
  editorCloseCurrentBuffer(editor);
}

/* FIXME: Must use WorldCmds */
void editorCmdFnPointToEndOfLine(Editor *editor, EditorCmd *editorCmd)
{
  editorPointToEndOfLine(editor);
}

/* FIXME: Must use WorldCmds */
void editorCmdFnPointToLineBegin(Editor *editor, EditorCmd *editorCmd)
{
  editorPointToLineBegin(editor);
}

void editorCmdFnWriteBuffer(Editor *editor, EditorCmd *editorCmd)
{
  editorWriteBuffer(editor);
}

void editorCmdFnPromptToOpenFile(Editor *editor, EditorCmd *editorCmd)
{
  char *filePath;
  filePath = editorRecoverFromPromptedInput(editor);
  editorOpenFile(editor, filePath);
}

void editorCmdFnOpenFile(Editor *editor, EditorCmd *editorCmd)
{
  char *filePath;
  if (editorCmd->paramSz) {
    filePath = parseStringParam(editorCmd);
    editorSetBufferFilePath(editor, filePath);
    editorOpenFile(editor, filePath);
  }
  else {
    editorPromptForInput(editor, "prompt_to_open_file", "Enter a file path...");
  }
}

void editorCmdFnToggleShareBuffer(Editor *editor, EditorCmd *editorCmd)
{
  editorShareBuffer(editor, !editorIsBufferShared(editor));
}

void editorCmdFnPromptToBindKeyCombo(Editor *editor, EditorCmd *editorCmd)
{
  char *params;
  params = editorRecoverFromPromptedInput(editor);
  editorCmd->param = params;
  editorCmd->paramSz = strlen(params);
  editorCmdFnBindKeyCombo(editor, editorCmd);
}

void editorCmdFnBindKeyCombo(Editor *editor, EditorCmd *editorCmd)
{
  char *params, *keyCombo, *cmd;
  int i, paramSz;
  short quoted;

  paramSz = editorCmd->paramSz;
  if (paramSz) {
    params = (char *)malloc(sizeof(char) * (paramSz + 1));
    strncpy(params, editorCmd->param, paramSz + 1);
    i = 0;
    quoted = (*params == '"');
    if (quoted) {
      params++;
      paramSz--;
    }
    do {
      if (quoted && (params[i] == '"') || (!quoted && params[i] == ' ')) {
        break;
      }
      else {
        i++;
      }
    } while(i < paramSz);

    if (i >= paramSz) {
      editorPromptForInput(editor, "prompt_to_bind", "Enter a command...");
      editorInsert(editor, editorCmd->param, editorCmd->paramSz);
      editorInsert(editor, " ", 1);
      free(params);
    }
    else {
      params[i++] = '\0';
      for (; params[i] == ' ' || params[i] == '"'; i++) {}
      params[paramSz] = '\0';
      editorBindKeyCombo(editor, params, params + i);
    }
  }
  else {
    editorPromptForInput(editor, "prompt_to_bind", "Bind what...?");
  }
}

void editorCmdFnCancel(Editor *editor, EditorCmd *editorCmd)
{
  editorCancel(editor);
}
