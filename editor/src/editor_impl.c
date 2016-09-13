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

#include "editor_impl.h"
#include <libsys/socket.h>
#include <uicore/keys.h>
#include "keybinding.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Editor *createEditor(UI *ui, char *startupMessage)
{
  Editor *editor;
  editor = (Editor *)malloc(sizeof(Editor));
  initEditor(editor, ui, startupMessage);
  return editor;
}

void editorCreateDefaultCommands_(Editor *editor)
{
  editorCmdHomeRegister(editor->editorCmdHome, "exit", editorCmdFnExit);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "cursor_right", editorCmdFnMoveRight);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "cursor_left", editorCmdFnMoveLeft);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "cursor_up", editorCmdFnMoveUp);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "cursor_down", editorCmdFnMoveDown);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prev_word", editorCmdFnPrevWord);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "next_word", editorCmdFnNextWord);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "backspace", editorCmdFnBackspace);
  editorCmdHomeRegister(editor->editorCmdHome, "insert", editorCmdFnInsert);
  editorCmdHomeRegister(editor->editorCmdHome, "delete", editorCmdFnDelete);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "line_feed", editorCmdFnLineFeed);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "insert_tab", editorCmdFnInsertTab);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "init_sharing", editorCmdFnInitSharing);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prompt_to_init_sharing",
                        editorCmdFnPromptToInitSharing);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "sharing_connect", editorCmdFnConnectToSharing);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prompt_to_sharing_connect",
                        editorCmdFnPromptToConnectToSharing);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "set_buffer", editorCmdFnChooseBuffer);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prompt_to_set_buffer", editorCmdFnPromptToSetBuffer);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "next_buffer", editorCmdFnNextBuffer);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prev_buffer", editorCmdFnPrevBuffer);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "split_window", editorCmdFnSplitWindowHorz);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "next_window", editorCmdFnNextWindow);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prev_window", editorCmdFnPrevWindow);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "remove_current_window",
                        editorCmdFnRemoveCurrentWindow);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "close_current_buffer", editorCmdFnCloseCurrentBuffer);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "point_to_end_of_line", editorCmdFnPointToEndOfLine);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "point_to_line_begin", editorCmdFnPointToLineBegin);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "write_buffer", editorCmdFnWriteBuffer);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "read_to_buffer", editorCmdFnOpenFile);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prompt_to_open_file", editorCmdFnPromptToOpenFile);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "toggle_share_current_buffer",
                        editorCmdFnToggleShareBuffer);
  editorCmdHomeRegister(editor->editorCmdHome, "bind", editorCmdFnBindKeyCombo);
  editorCmdHomeRegister(editor->editorCmdHome,
                        "prompt_to_bind", editorCmdFnPromptToBindKeyCombo);
  editorCmdHomeRegister(editor->editorCmdHome, "cancel", editorCmdFnCancel);
}

void editorBindKeyCombo(Editor *editor, char *keyCombo, char *cmdStr)
{
  EditorCmd *editorCmd;
  KbInput seq[KEY_COMBO_MAX_SIZE];
  int paramSz;
  char *command;
  char *param;
  int n, i;
  n = 0;
  param = NULL;

  while (*keyCombo) {
    if (*keyCombo == ' ') {
      keyCombo++;
      continue;
    }
    seq[n++] = *(kbInputParse(&keyCombo));
  }
  while (n < KEY_COMBO_MAX_SIZE) {
    seq[n++].key = NO_KEY;
  }

  i = 0;
  command = (char *)malloc(sizeof(char) * 64);
  while ((*cmdStr != '\0') && (*cmdStr != ' ')) {
    command[i++] = *(cmdStr++);
  }
  command[i] = '\0';
  if (*cmdStr == ' ') {
    ++cmdStr;
    paramSz = strlen(cmdStr);
  }
  else {
    paramSz = 0;
  }

  if (paramSz) {
    param = strcpy((char *)malloc(sizeof(char) * strlen(cmdStr)), cmdStr);
  }
  editorCmd = editorCmdHomeCreateCmd(editor->editorCmdHome, command,
                                     paramSz, param);
  editorCmdTreeBind(editor->editorCmdTree, seq, editorCmd);
}

EditorCmdTree *generateEditorDefaultKeyBindings(Editor *editor)
{
  editorBindKeyCombo(editor, "M-q", "exit");
  editorBindKeyCombo(editor, "rightarrow", "cursor_right");
  editorBindKeyCombo(editor, "C-d", "cursor_right");
  editorBindKeyCombo(editor, "leftarrow", "cursor_left");
  editorBindKeyCombo(editor, "C-a", "cursor_left");
  editorBindKeyCombo(editor, "uparrow", "cursor_up");
  editorBindKeyCombo(editor, "C-w", "cursor_up");
  editorBindKeyCombo(editor, "downarrow", "cursor_down");
  editorBindKeyCombo(editor, "C-s", "cursor_down");
  editorBindKeyCombo(editor, "C-c", "prev_word");
  editorBindKeyCombo(editor, "C-v", "next_word");
  editorBindKeyCombo(editor, "C-b", "set_buffer");
  editorBindKeyCombo(editor, "backspace", "backspace");
  editorBindKeyCombo(editor, "delete", "delete");
  editorBindKeyCombo(editor, "tab", "insert_tab");
  editorBindKeyCombo(editor, "return", "line_feed");
  editorBindKeyCombo(editor, "M-l", "init_sharing");
  editorBindKeyCombo(editor, "M-c", "sharing_connect");
  editorBindKeyCombo(editor, "M-m", "toggle_share_current_buffer");
  editorBindKeyCombo(editor, "C-n", "next_buffer");
  editorBindKeyCombo(editor, "C-p", "prev_buffer");
  editorBindKeyCombo(editor, "M-s", "split_window");
  editorBindKeyCombo(editor, "M-o", "next_window");
  editorBindKeyCombo(editor, "M-p", "prev_window");
  editorBindKeyCombo(editor, "M-k", "remove_current_window");
  editorBindKeyCombo(editor, "C-k", "close_current_buffer");
  editorBindKeyCombo(editor, "C-e", "point_to_end_of_line");
  editorBindKeyCombo(editor, "C-q", "point_to_line_begin");
  editorBindKeyCombo(editor, "M-,", "read_to_buffer");
  editorBindKeyCombo(editor, "M-.", "write_buffer");
  editorBindKeyCombo(editor, "C-l C-k", "insert Ut Queant laxis\nresonare fibris,\nMira gestorum\nfamuli tuorum,\nSolve polluti\nlabii reatum,\nSancte Iohannes\n");
  editorBindKeyCombo(editor, "M-b", "bind");
  /* editorBindKeyCombo(editor, "C-l l", "insert int main(int argc, char *argv[])\n{\n\n  return 0;\n}\n"); */
  editorBindKeyCombo(editor, "C-g", "cancel");
}

void initEditor(Editor *editor, UI *ui, char *startupMessage)
{
  World *world;
  SharingServer *sharingServer;

  world = createWorld(ui);
  /* Create auxiliary buffers */
  worldCreateBuffer(world, "*prompt*");
  worldCreateBuffer(world, "*messages*");

  windowSetBufferName(uiGetWindow(ui), worldGetBufferName(world));

  sharingServer = (SharingServer *)malloc(sizeof(SharingServer));
  initSharingServer(sharingServer, world, DEFAULT_SHARING_PORT);
  sharingServerCreateBufferServer(sharingServer, NULL);

  editor->world = world;
  editor->ui = ui;
  editor->sharingServer = sharingServer;
  editor->editorCmdHome = (EditorCmdHome *)malloc(sizeof(EditorCmdHome));
  initEditorCmdHome(editor->editorCmdHome);
  editorCreateDefaultCommands_(editor);
  editor->editorCmdTree = (EditorCmdTree *)malloc(sizeof(EditorCmdTree));
  initEditorCmdTree(editor->editorCmdTree);
  generateEditorDefaultKeyBindings(editor);

  uiSetWindowHasStatusLine(ui, 1);
  uiSayCentered(ui, startupMessage);
  uiRedisplay(ui, world);
  uiSetObserver(ui, editor);
}

void destroyEditor(Editor *editor)
{
  destroySharingServer(editor->sharingServer);
  destroyEditorCmdHome(editor->editorCmdHome);
  free(editor->sharingServer);
  free(editor->editorCmdHome);
  free(editor->world);
}

void editorShowMessage(Editor *editor, char *msg)
{
  char *bufferName;
  bufferName = worldGetBufferName(editor->world);

  worldSetCurrentBuffer(editor->world, "*messages*", 1);
  worldSetPoint(editor->world, worldBufferSize(editor->world));
  worldInsert(editor->world, (Byte *)msg, strlen(msg));
  worldInsert(editor->world, (Byte *)"\n", 1);
  worldSetCurrentBuffer(editor->world, bufferName, 0);
  uiRedisplay(editor->ui, editor->world);
}

void editorRun(Editor *editor)
{
  uiMainLoop(editor->ui);
}


void editorSetBufferShared_(Editor *editor, short shared)
{
  worldSetBufferFlag(editor->world, BUFFER_FLAG_SHARED, shared);
}

#define editorProcessWorldCommand_(editor, cmd, multicast) (multicast ? sharingServerMulticast(((Editor *)editor)->sharingServer, cmd) : worldCmdExecute(cmd, ((Editor *)editor)->world))

/* It is the UI callback function */
UiFeedback editorKeyHandle(Editor *editor, KbInput *kbInput)
{
  EditorCmdTree *editorCmdTreeRoot;
  static EditorCmdTree *editorCmdTree = NULL;
  EditorCmd *editorCmd;

  /* Clears everything that the editor could have said. */
  uiSayCentered(editor->ui, "");

  editorCmdTreeRoot = editor->editorCmdTree;

  if (editorCmdTree == NULL) {
    editorCmdTree = editorCmdTreeRoot;
  }

  worldSetCurrentBuffer(editor->world, uiGetWindowBufferName(editor->ui), 0);

  if (editorCmdTree == editorCmdTreeRoot) {
    if (kbInput->key == KEY_SYMBOL) {
      editorProcessWorldCommand_(editor,
                                 createWorldCmd(WORLDCMD_INSERT,
                                                worldGetPoint(editor->world),
                                                kbInput->size, kbInput->size,
                                                kbInput->info,
                                                uiGetWindowBufferName(editor->ui)
                                                ),
                                 editorIsBufferShared(editor));
      return worldIsAlive(editor->world) ? UI_CONTINUE : UI_STOP;
    }
  }

  editorCmdTree = editorCmdTreeGet(editorCmdTree, kbInput);
  if (editorCmdTree == NULL) {
    editorShowMessage(editor, "Key not bound");
    editorCmdTree = editorCmdTreeRoot;
  }
  else {
    editorCmd = editorCmdTreeGetCmd(editorCmdTree);
    if (editorCmd != NULL) {
      worldSetCurrentBuffer(editor->world, uiGetWindowBufferName(editor->ui), 0);
      editorCmdExecute(editorCmd, editor);
      editorCmdTree = editorCmdTreeRoot;
    }
  }

  return worldIsAlive(editor->world) ? UI_CONTINUE : UI_STOP;
}

void editorExit(Editor *editor)
{
  if (sharingServerIsListening(editor->sharingServer)) {
    editorStopSharing(editor);
  }
  sharingServerMulticast(editor->sharingServer,
                         createWorldCmd(WORLDCMD_DIE,
                                        worldGetPoint(editor->world),
                                        0, 0, NULL, NULL));
}

void editorInsert(Editor *editor, Byte *info, Size size)
{
  editorProcessWorldCommand_(editor,
                          createWorldCmd(WORLDCMD_INSERT,
                                         worldGetPoint(editor->world),
                                         size, size, info,
                                         worldGetBufferName(editor->world)),
                          editorIsBufferShared(editor));
}

void editorBackspace(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, 1);

  editorProcessWorldCommand_(editor,
                          createWorldCmd(WORLDCMD_DELETE,
                                         pnew, pold - pnew, 0, NULL,
                                         worldGetBufferName(editor->world)),
                          editorIsBufferShared(editor));
}

void editorDelete(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pnew = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, 1);
  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -1);

  editorProcessWorldCommand_(editor,
                          createWorldCmd(WORLDCMD_DELETE,
                                         worldGetPoint(editor->world),
                                         pold - pnew, 0, NULL,
                                         worldGetBufferName(editor->world)),
                          editorIsBufferShared(editor));
}

void editorLineFeed(Editor *editor)
{
  char *lf;
  /* FIXME: put this in a better place */
  static char lfCh[] = "\n";
  static int lfSz = 0;
  if (!lfSz) lfSz = sizeof(char) * strlen(lfCh);

  lf = (char *)malloc(lfSz);
  memcpy(lf, lfCh, lfSz);

  editorProcessWorldCommand_(editor,
                          createWorldCmd(WORLDCMD_INSERT,
                                         worldGetPoint(editor->world),
                                         lfSz, lfSz, lf,
                                         worldGetBufferName(editor->world)),
                          editorIsBufferShared(editor));
}

void editorInsertTab(Editor *editor)
{
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_INSERT,
                                            worldGetPoint(editor->world),
                                            2, 2, "  ",
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorMoveLeft(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, 1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_BACKWARD,
                                            worldGetPoint(editor->world),
                                            pold - pnew, 0, NULL,
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorMoveRight(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, 1);
  pnew = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_FORWARD,
                                            worldGetPoint(editor->world),
                                            pnew - pold, 0, NULL,
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorMoveUp(Editor *editor)
{
  Position pnew;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveLineBufferPointFn(editor->world, 1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_SET_POINT,
                                            pnew, 0, 0, NULL,
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorMoveDown(Editor *editor)
{
  Position pnew;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, 1);
  pnew = worldGetPoint(editor->world);
  moveLineBufferPointFn(editor->world, -1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_SET_POINT,
                                            pnew, 0, 0, NULL,
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorPrevWord(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveWordBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveWordBufferPointFn = modeGetBufferPointWordFn(mode);

  pold = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, 1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_BACKWARD,
                                            pold,
                                            pold - pnew, 0, NULL,
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorNextWord(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveWordBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));
  moveWordBufferPointFn = modeGetBufferPointWordFn(mode);

  pold = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, 1);
  pnew = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, -1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_FORWARD,
                                            pold,
                                            pnew - pold, 0, NULL,
                                            worldGetBufferName(editor->world)),
                             editorIsBufferShared(editor));
}

void editorConnectToSharing(Editor *editor, char *host, int port)
{
  Socket *s;
  s = createSocket(host, port);
  sharingServerConnect(editor->sharingServer, s);
}

void editorPromptForInput(Editor *editor, char *editorCmdStr, char *label)
{
  EditorCmd *editorCmd;
  KbInput kbInput[2];
  size_t labelLen;
  labelLen = strlen(label);
  uiActivateMiniWindow(editor->ui, 1);
  uiSetWindowBuffer(editor->ui, "*prompt*");
  worldSetCurrentBuffer(editor->world, "*prompt*", 1);
  worldInsert(editor->world, label, labelLen);

  kbInput[0].key = KEY_RETURN;
  kbInput[0].info = NULL;
  kbInput[0].size = 0;
  kbInput[1].key = NO_KEY;
  editorCmd = editorCmdHomeCreateCmd(editor->editorCmdHome,
                                     editorCmdStr, 0, NULL);
  editorCmdTreeBind(editor->editorCmdTree, kbInput, editorCmd);
  /* FIXME: Why this doesn't work? */
  /* editorBindKeyCombo(editor, "return", editorCmdStr); */
  uiRedisplay(editor->ui, editor->world);
  worldSetCurrentBuffer(editor->world, "*prompt*", 1);
  worldSetPoint(editor->world, 0);
  worldDelete(editor->world, labelLen);
}

char *editorRecoverFromPromptedInput(Editor *editor)
{
  KbInput kbInput[2];
  char *input;
  Size sz;
  EditorCmd *editorCmd;
  char *prevBuffer;

  prevBuffer = worldGetBufferName(editor->world);
  worldSetCurrentBuffer(editor->world, "*prompt*", 1);
  sz = worldBufferSize(editor->world);
  input = (char *)malloc(sizeof(char) * (sz + 1));
  worldSetPoint(editor->world, 0);
  worldGetChunk(editor->world, input, sz);
  worldDelete(editor->world, sz);
  input[sz] = '\0';
  worldSetCurrentBuffer(editor->world, prevBuffer, 0);
  uiActivateMiniWindow(editor->ui, 1);
  uiSetWindowBuffer(editor->ui, "*messages*");
  uiActivateMiniWindow(editor->ui, 0);
  worldNotifyObservers(editor->world);

  kbInput[0].key = KEY_RETURN;
  kbInput[0].info = NULL;
  kbInput[0].size = 0;
  kbInput[1].key = NO_KEY;
  editorCmd = editorCmdHomeCreateCmd(editor->editorCmdHome,
                                     "line_feed", 0, NULL);
  editorCmdTreeBind(editor->editorCmdTree, kbInput, editorCmd);

  /* TODO: Why this doesn't work? */
  /* editorBindKeyCombo(editor, "return", "line_feed"); */

  return input;
}

void editorChooseBuffer(Editor *editor, char *bufferName)
{
  assert(bufferName != NULL);

  worldSetCurrentBuffer(editor->world, bufferName, 1);
  uiSetWindowBuffer(editor->ui, bufferName);
  worldNotifyObservers(editor->world);
}

void editorSplitWindowHorz(Editor *editor)
{
  uiSplitWindowHorz(editor->ui);
  uiNextWindow(editor->ui);
  uiSetWindowHasStatusLine(editor->ui, 1);
  uiPrevWindow(editor->ui);
  uiRedisplay(editor->ui, editor->world);
}

void editorRemoveCurrentWindow(Editor *editor)
{
  uiRemoveCurrentWindow(editor->ui);
  uiRedisplay(editor->ui, editor->world);
}

void editorNextWindow(Editor *editor)
{
  uiNextWindow(editor->ui);
  worldSetCurrentBuffer(editor->world, uiGetWindowBufferName(editor->ui), 0);
  uiRedisplay(editor->ui, editor->world);
}

void editorPrevWindow(Editor *editor)
{
  uiPrevWindow(editor->ui);
  worldSetCurrentBuffer(editor->world, uiGetWindowBufferName(editor->ui), 0);
  uiRedisplay(editor->ui, editor->world);
}

void editorInitSharing(Editor *editor)
{
  sharingServerListen(editor->sharingServer);
  editorSetBufferShared_(editor, 1);
}

void editorStopSharing(Editor *editor)
{
  sharingServerStopListening(editor->sharingServer);
}

void editorSetSharingPort(Editor *editor, int port)
{
  sharingServerSetPort(editor->sharingServer, port);
}

void editorNextBuffer(Editor *editor)
{
  worldNextBuffer(editor->world);
  uiSetWindowBuffer(editor->ui, worldGetBufferName(editor->world));
  worldNotifyObservers(editor->world);
}

void editorPrevBuffer(Editor *editor)
{
  worldPrevBuffer(editor->world);
  uiSetWindowBuffer(editor->ui, worldGetBufferName(editor->world));
  worldNotifyObservers(editor->world);
}

void editorCloseCurrentBuffer(Editor *editor)
{
  char *bufName;
  bufName = worldGetBufferName(editor->world);
  worldNextBuffer(editor->world);
  if (!strcmp(bufName, worldGetBufferName(editor->world))) {
    editorShowMessage(editor, "The last buffer cannot be closed.");
    return;
  }
  /* FIXME: passing 1 as create arg only because it would
     cause less trouble under an inconsistent state */
  worldSetCurrentBuffer(editor->world, uiGetWindowBufferName(editor->ui), 1);
  worldCloseBuffer(editor->world);
  uiSetWindowBuffer(editor->ui, worldGetBufferName(editor->world));
  worldNotifyObservers(editor->world);
}

/* FIXME: Must use WorldCmds */
void editorPointToEndOfLine(Editor *editor)
{
  MoveBufferPointFn moveBufferPointFn;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));

  moveBufferPointFn = modeGetBufferPointFn(mode);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, 1);
  if (worldGetPoint(editor->world) < (worldBufferSize(editor->world) - 1))
    {
      moveBufferPointFn(editor->world, -1);
    }
  worldNotifyObservers(editor->world);
}

/* FIXME: Must use WorldCmds */
void editorPointToLineBegin(Editor *editor)
{
  MoveBufferPointFn moveBufferPointFn;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  mode = windowGetMode(uiGetWindow(editor->ui));

  moveBufferPointFn = modeGetBufferPointFn(mode);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, -1);
  moveLineBufferPointFn(editor->world, 1);

  worldNotifyObservers(editor->world);
}

void editorWriteBuffer(Editor *editor)
{
  if (!strlen(worldGetBufferFilePath(editor->world))) {
    worldSetBufferFilePath(editor->world,
                           worldGetBufferName(editor->world));
  }
  worldWriteBuffer(editor->world);
}

char *fileName(char *fpath)
{
  char *fname;

  fname = fpath;
  while (*fpath != '\0') {
    if (*fpath == '\\') {
      fname = fpath;
    }
    fpath += 1;
  }

  return fname;
}

void editorOpenFile(Editor *editor, char *filePath)
{
  char *bufferName;
  bufferName = fileName(filePath);
  worldCreateBuffer(editor->world, bufferName);
  worldSetCurrentBuffer(editor->world, bufferName, 1);
  worldSetBufferFilePath(editor->world, filePath);
  worldReadBuffer(editor->world);
  uiSetWindowBuffer(editor->ui, bufferName);
  worldNotifyObservers(editor->world);
}

void editorGetBufferFilePath(Editor *editor)
{
  worldGetBufferFilePath(editor->world);
}

void editorSetBufferFilePath(Editor *editor, char *filePath)
{
  worldSetBufferFilePath(editor->world, filePath);
}

void editorShareBuffer(Editor *editor, int share)
{
  sharingServerShareBuffer(editor->sharingServer);
  editorSetBufferShared_(editor, share);
  worldNotifyObservers(editor->world);
}

int editorIsBufferShared(Editor *editor)
{
  return worldGetBufferFlags(editor->world) & BUFFER_FLAG_SHARED;
}

void editorCancel(Editor *editor)
{
  editorRecoverFromPromptedInput(editor);
}
