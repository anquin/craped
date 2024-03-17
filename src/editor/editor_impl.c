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
#include "editorcmd-funcs.h"
#include "frontend.h"
#include <libsys/mem.h>
#include <libsys/socket.h>
#include <libsys/strhash.h>
#include <uicore/keys.h>
#include "keybinding.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Editor *createEditor(void *ui,
                     EditorSubscriber **subscribers,
                     const char *startupMessage)
{
  Editor *editor;
  editor = (Editor *)malloc(sizeof(Editor));
  initEditor(editor, (UI *)ui, subscribers, startupMessage);
  return editor;
}

void editorRegisterCommand(Editor *editor, char *cmdStr, void *fn)
{
  EditorCmdDeclaration *cmdDecl;
  cmdDecl = createEditorCmdDeclaration(cmdStr, (EditorCmdExecuteFn)fn);
  editorCmdHomeRegister(editor->editorCmdHome, cmdDecl);
}

void editorRegisterExtensionCommand(Editor *editor, const char *editorExtKey,
                                    char *cmdStr, void *fn)
{
  EditorCmdDeclaration *cmdDecl;
  cmdDecl =
    createEditorExtensionCmdDeclaration(editorExtKey,
                                        cmdStr,
                                        (EditorExtensionCmdExecuteFn)fn);
  editorCmdHomeRegister(editor->editorCmdHome, cmdDecl);
}

void editorCreateDefaultCommands_(Editor *editor)
{
  editorRegisterCommand(editor, "exit", editorCmdFnExit);
  editorRegisterCommand(editor, "cursor_right", editorCmdFnMoveRight);
  editorRegisterCommand(editor, "cursor_left", editorCmdFnMoveLeft);
  editorRegisterCommand(editor, "cursor_up", editorCmdFnMoveUp);
  editorRegisterCommand(editor, "cursor_down", editorCmdFnMoveDown);
  editorRegisterCommand(editor, "page_up", editorCmdFnPageUp);
  editorRegisterCommand(editor, "page_down", editorCmdFnPageDown);
  editorRegisterCommand(editor, "prev_word", editorCmdFnPrevWord);
  editorRegisterCommand(editor, "next_word", editorCmdFnNextWord);
  editorRegisterCommand(editor, "backspace", editorCmdFnBackspace);
  editorRegisterCommand(editor, "insert", editorCmdFnInsert);
  editorRegisterCommand(editor, "delete", editorCmdFnDelete);
  editorRegisterCommand(editor, "line_feed", editorCmdFnLineFeed);
  editorRegisterCommand(editor, "insert_tab", editorCmdFnInsertTab);
  editorRegisterCommand(editor, "init_sharing", editorCmdFnInitSharing);
  editorRegisterCommand(editor, "prompt_to_init_sharing",
                        editorCmdFnPromptToInitSharing);
  editorRegisterCommand(editor, "sharing_connect", editorCmdFnConnectToSharing);
  editorRegisterCommand(editor, "prompt_to_sharing_connect",
                        editorCmdFnPromptToConnectToSharing);
  editorRegisterCommand(editor, "set_buffer", editorCmdFnChooseBuffer);
  editorRegisterCommand(editor, "set_window_buffer", editorCmdFnChooseWindowBuffer);
  editorRegisterCommand(editor, "prompt_to_set_window_buffer",
                        editorCmdFnPromptToSetWindowBuffer);
  editorRegisterCommand(editor, "next_buffer", editorCmdFnNextBuffer);
  editorRegisterCommand(editor, "prev_buffer", editorCmdFnPrevBuffer);
  editorRegisterCommand(editor, "hsplit_window", editorCmdFnSplitWindowHorz);
  editorRegisterCommand(editor, "vsplit_window", editorCmdFnSplitWindowVert);
  editorRegisterCommand(editor, "next_window", editorCmdFnNextWindow);
  editorRegisterCommand(editor, "prev_window", editorCmdFnPrevWindow);
  editorRegisterCommand(editor, "remove_current_window",
                        editorCmdFnRemoveCurrentWindow);
  editorRegisterCommand(editor, "close_current_buffer",
                        editorCmdFnCloseCurrentBuffer);
  editorRegisterCommand(editor,
                        "point_to_end_of_line", editorCmdFnPointToEndOfLine);
  editorRegisterCommand(editor,
                        "point_to_line_begin", editorCmdFnPointToLineBegin);
  editorRegisterCommand(editor, "write_buffer", editorCmdFnWriteBuffer);
  editorRegisterCommand(editor, "read_to_buffer", editorCmdFnOpenFile);
  editorRegisterCommand(editor,
                        "prompt_to_open_file", editorCmdFnPromptToOpenFile);
  editorRegisterCommand(editor,
                        "toggle_buffer_rdonly", editorCmdFnToggleReadOnly);
  editorRegisterCommand(editor,
                        "toggle_share_current_buffer",
                        editorCmdFnToggleShareBuffer);
  editorRegisterCommand(editor, "bind", editorCmdFnBindKeyCombo);
  editorRegisterCommand(editor,
                        "prompt_to_bind", editorCmdFnPromptToBindKeyCombo);
  editorRegisterCommand(editor, "cancel", editorCmdFnCancel);
  editorRegisterCommand(editor, "undo", editorCmdFnUndo);
  editorRegisterCommand(editor, "redraw_entire_screen",
                        editorCmdFnRedrawEntireScreen);
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
  editorBindKeyCombo(editor, "M-a", "page_up");
  editorBindKeyCombo(editor, "M-d", "page_down");
  editorBindKeyCombo(editor, "C-s", "cursor_down");
  editorBindKeyCombo(editor, "C-c", "point_to_line_begin");
  editorBindKeyCombo(editor, "C-v", "point_to_end_of_line");
  editorBindKeyCombo(editor, "C-b", "set_window_buffer");
  editorBindKeyCombo(editor, "C-e", "next_word");
  editorBindKeyCombo(editor, "C-q", "prev_word");
  editorBindKeyCombo(editor, "backspace", "backspace");
  editorBindKeyCombo(editor, "C-h", "backspace");
  editorBindKeyCombo(editor, "delete", "delete");
  editorBindKeyCombo(editor, "C-x", "delete");
  editorBindKeyCombo(editor, "tab", "insert_tab");
  editorBindKeyCombo(editor, "return", "line_feed");
  editorBindKeyCombo(editor, "M-j", "line_feed");
  editorBindKeyCombo(editor, "M-l", "init_sharing");
  editorBindKeyCombo(editor, "M-c", "sharing_connect");
  editorBindKeyCombo(editor, "M-r", "toggle_buffer_rdonly");
  editorBindKeyCombo(editor, "M-m", "toggle_share_current_buffer");
  editorBindKeyCombo(editor, "C-n", "next_buffer");
  editorBindKeyCombo(editor, "C-p", "prev_buffer");
  editorBindKeyCombo(editor, "M-s", "hsplit_window");
  editorBindKeyCombo(editor, "M-v", "vsplit_window");
  editorBindKeyCombo(editor, "M-o", "next_window");
  editorBindKeyCombo(editor, "M-n", "next_window");
  editorBindKeyCombo(editor, "M-p", "prev_window");
  editorBindKeyCombo(editor, "M-k", "remove_current_window");
  editorBindKeyCombo(editor, "C-k", "close_current_buffer");
  editorBindKeyCombo(editor, "M-,", "read_to_buffer");
  editorBindKeyCombo(editor, "M-.", "write_buffer");
  editorBindKeyCombo(editor, "C-l C-k", "insert Ut Queant laxis\nresonare fibris,\nMira gestorum\nfamuli tuorum,\nSolve polluti\nlabii reatum,\nSancte Iohannes\n");
  editorBindKeyCombo(editor, "M-b", "bind");
  /* editorBindKeyCombo(editor, "C-l l", "insert int main(int argc, char *argv[])\n{\n\n  return 0;\n}\n"); */
  editorBindKeyCombo(editor, "C-g", "cancel");
  editorBindKeyCombo(editor, "C-z", "undo");
  editorBindKeyCombo(editor, "M-u", "redraw_entire_screen");
}

void initEditor(Editor *editor,
                void *ui,
                EditorSubscriber **subscribers,
                const char *startupMessage)
{
  World *world;
  SharingServer *sharingServer;
  EditorFrontend *frontend;

  frontend = createEditorFrontend((UI *)ui);
  while (*subscribers != NULL) {
    editorFrontendAddSubscriber(frontend, *(subscribers++));
  }
  world = createWorld(frontend);
  windowSetBufferName(uiGetActiveWindow((UI *)ui), worldGetBufferName(world));

  /* Create auxiliary buffers */
  worldSetCurrentBuffer(world, "*messages*", 1);
  worldSetBufferFlag(world, WORLD_BUFFER_FLAG_RDONLY, 1);

  sharingServer = (SharingServer *)malloc(sizeof(SharingServer));
  initSharingServer(sharingServer, world, DEFAULT_SHARING_PORT);
  sharingServerCreateBufferServer(sharingServer, NULL);

  editor->world = world;
  editor->ui = (UI *)ui;
  editor->sharingServer = sharingServer;
  editor->editorCmdHome = createEditorCmdHome();
  editorCreateDefaultCommands_(editor);
  editor->startupMessage = startupMessage;
  editor->editorCmdTree = (EditorCmdTree *)malloc(sizeof(EditorCmdTree));
  initEditorCmdTree(editor->editorCmdTree);
  generateEditorDefaultKeyBindings(editor);

  uiSetWindowHasStatusLine((UI *)ui, 1);
  uiSetObserver((UI *)ui, editor);

  editor->extensions = (HashTable *)malloc(sizeof(HashTable));
  hash_table_init(editor->extensions, 257);  /* 257 is prime */
}

void destroyEditor(Editor *editor)
{
  destroySharingServer(editor->sharingServer);
  destroyEditorCmdHome(editor->editorCmdHome);
  hash_table_fini(editor->extensions);
  free(editor->sharingServer);
  free(editor->editorCmdHome);
  free(editor->extensions);
  free(editor->world);
}

void editorShowMessage(Editor *editor, char *msg, short lf)
{
  char *bufferName;
  bufferName = worldGetBufferName(editor->world);

  worldSetCurrentBuffer(editor->world, "*messages*", 1);
  worldSetPoint(editor->world, worldBufferSize(editor->world));
  worldSetBufferFlag(editor->world, WORLD_BUFFER_FLAG_RDONLY, 0);
  worldInsert(editor->world, (Byte *)msg, strlen(msg));
  if (lf) {
    worldInsert(editor->world, (Byte *)"\n", 1);
  }
  worldSetBufferFlag(editor->world, WORLD_BUFFER_FLAG_RDONLY, 1);
  worldSetCurrentBuffer(editor->world, bufferName, 0);
  worldNotifyObservers(editor->world);
}

void editorUndo(Editor *editor)
{
  worldUndo(editor->world);
  worldNotifyObservers(editor->world);
}

void editorRun(Editor *editor)
{
  uiBegin(editor->ui);
  uiSayCentered(editor->ui, editor->startupMessage);
  uiRedisplay(editor->ui, editor->world);
  uiMainLoop(editor->ui);
  uiEnd(editor->ui);
}


void editorSetBufferShared_(Editor *editor, short shared)
{
  worldSetBufferFlag(editor->world, BUFFER_FLAG_SHARED, shared);
}

void editorPostProcessWorldCommand_(Editor *editor, WorldCmd *cmd)
{
  UInt32 bufFlags;
  char *cmdBuffer;

  cmdBuffer = worldCmdGetBufferName(cmd);
  if (!worldBufferExists(editor->world, cmdBuffer)) {
    editorShowMessage(editor, "Buffer ", 0);
    editorShowMessage(editor, cmdBuffer, 0);
    editorShowMessage(editor, " died", 1);
  }
  else {
    worldSetCurrentBuffer(editor->world, cmdBuffer, 0);
    bufFlags = worldGetBufferFlags(editor->world);
    if (bufFlags & WORLD_BUFFER_FLAG_EOB) {
      editorShowMessage(editor, "End of buffer", 1);
    }
    else if (bufFlags & WORLD_BUFFER_FLAG_BOB) {
      editorShowMessage(editor, "Beggining of buffer", 1);
    }
  }
}

#define editorProcessWorldCommand_(editor, cmd, multicast) (multicast ? sharingServerMulticast(((Editor *)editor)->sharingServer, cmd) : worldCmdExecute(cmd, ((Editor *)editor)->world));editorPostProcessWorldCommand_(editor, cmd)

/* It is the UI callback function */
UiFeedback editorKeyHandle(Editor *editor, KbInput *kbInput)
{
  EditorCmdTree *editorCmdTreeRoot;
  static EditorCmdTree *editorCmdTree = NULL;
  EditorCmd *editorCmd;
  Window *activeWnd;
  char *activeWndBufName;

  /* Clears everything that the editor could have said. */
  uiSayCentered(editor->ui, "");

  editorCmdTreeRoot = editor->editorCmdTree;

  if (editorCmdTree == NULL) {
    editorCmdTree = editorCmdTreeRoot;
  }

  activeWnd = uiGetActiveWindow(editor->ui);
  activeWndBufName = uiGetWindowBufferName(editor->ui, activeWnd);
  worldSetCurrentBuffer(editor->world, activeWndBufName, 0);

  if (editorCmdTree == editorCmdTreeRoot) {
    if (kbInput->key == KEY_SYMBOL) {
      editorProcessWorldCommand_(editor,
                                 createWorldCmd(WORLDCMD_INSERT,
                                                worldGetPoint(editor->world),
                                                kbInput->size, kbInput->size,
                                                kbInput->info,
                                                activeWndBufName),
                                 editorIsBufferShared(editor));
      return worldIsAlive(editor->world) ? UI_CONTINUE : UI_STOP;
    }
  }

  editorCmdTree = editorCmdTreeGet(editorCmdTree, kbInput);
  if (editorCmdTree == NULL) {
    editorShowMessage(editor, "Key not bound", 1);
    editorCmdTree = editorCmdTreeRoot;
  }
  else {
    editorCmd = editorCmdTreeGetCmd(editorCmdTree);
    if (editorCmd != NULL) {
      worldSetCurrentBuffer(editor->world, activeWndBufName, 0);
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
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, 1);

  editorProcessWorldCommand_(editor,
                          createWorldCmd(WORLDCMD_DELETE,
                                         pnew, pold - pnew, 0, NULL,
                                         currBuffer),
                          editorIsBufferShared(editor));
}

void editorDelete(Editor *editor, Size size)
{
  Position pold, pnew;
  MoveBufferPointFn moveBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pnew = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, size);
  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -size);

  editorProcessWorldCommand_(editor,
                          createWorldCmd(WORLDCMD_DELETE,
                                         worldGetPoint(editor->world),
                                         pold - pnew, 0, NULL,
                                         currBuffer),
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
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  if (!(worldGetBufferFlags(editor->world) & WORLD_BUFFER_FLAG_BOB)) {
    moveBufferPointFn(editor->world, 1);
  }
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_BACKWARD,
                                            worldGetPoint(editor->world),
                                            pold - pnew, 0, NULL,
                                            currBuffer),
                             editorIsBufferShared(editor));
}

void editorMoveRight(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveBufferPointFn = modeGetBufferPointFn(mode);

  pold = worldGetPoint(editor->world);
  moveBufferPointFn(editor->world, 1);
  pnew = worldGetPoint(editor->world);
  if (!(worldGetBufferFlags(editor->world) & WORLD_BUFFER_FLAG_EOB)) {
    moveBufferPointFn(editor->world, -1);
  }
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_FORWARD,
                                            worldGetPoint(editor->world),
                                            pnew - pold, 0, NULL,
                                            currBuffer),
                             editorIsBufferShared(editor));
}

void editorMoveUp(Editor *editor)
{
  Position pnew;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveLineBufferPointFn(editor->world, 1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_SET_POINT,
                                            pnew, 0, 0, NULL,
                                            currBuffer),
                             editorIsBufferShared(editor));
}

void editorMoveDown(Editor *editor)
{
  Position pnew;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, 1);
  pnew = worldGetPoint(editor->world);
  moveLineBufferPointFn(editor->world, -1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_SET_POINT,
                                            pnew, 0, 0, NULL,
                                            currBuffer),
                             editorIsBufferShared(editor));
}

static void editorMovePage_(Editor *editor, int n)
{
  Mode *mode;
  Position pnew;
  Window *currWnd;
  char *currBuffer;
  unsigned i, win_size_x, win_size_y;
  MoveBufferPointFn moveLineBufferPointFn;

  currBuffer = worldGetBufferName(editor->world);

  currWnd = uiGetActiveWindow(editor->ui);
  mode = windowGetMode(currWnd);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  worldAddMark(editor->world, "*PGMV*");;
  worldSetPoint(editor->world, windowGetTop(currWnd));
  uiGetWindowSize(editor->ui, &win_size_x, &win_size_y);
  if (n < 0) {
    moveLineBufferPointFn(editor->world, win_size_y);
  }
  moveLineBufferPointFn(editor->world, n * (win_size_y + win_size_y / 2));
  pnew = worldGetPoint(editor->world);
  worldMarkToPoint(editor->world, "*PGMV*");
  worldRemoveMark(editor->world, "*PGMV*");

  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_SET_POINT,
                                            pnew, 0, 0, NULL,
                                            currBuffer),
                             editorIsBufferShared(editor));
}

void editorPageUp(Editor *editor)
{
  editorMovePage_(editor , -1);
}

void editorPageDown(Editor *editor)
{
  editorMovePage_(editor , 1);
}

void editorPrevWord(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveWordBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveWordBufferPointFn = modeGetBufferPointWordFn(mode);

  pold = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, -1);
  pnew = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, 1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_BACKWARD,
                                            pold,
                                            pold - pnew, 0, NULL,
                                            currBuffer),
                             editorIsBufferShared(editor));
}

void editorNextWord(Editor *editor)
{
  Position pold, pnew;
  MoveBufferPointFn moveWordBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);
  moveWordBufferPointFn = modeGetBufferPointWordFn(mode);

  pold = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, 1);
  pnew = worldGetPoint(editor->world);
  moveWordBufferPointFn(editor->world, -1);
  editorProcessWorldCommand_(editor,
                             createWorldCmd(WORLDCMD_MOVE_POINT_FORWARD,
                                            pold,
                                            pnew - pold, 0, NULL,
                                            currBuffer),
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
  Window *activeWnd;

  labelLen = strlen(label);
  uiActivateMiniWindow(editor->ui, 1);
  activeWnd = uiGetActiveWindow(editor->ui);
  uiSetWindowBufferName(editor->ui, activeWnd, "*prompt*");
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
  worldNotifyObservers(editor->world);
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
  Window *activeWnd;

  prevBuffer = worldGetBufferName(editor->world);
  worldSetCurrentBuffer(editor->world, "*prompt*", 1);
  sz = worldBufferSize(editor->world);
  input = (char *)malloc(sizeof(char) * (sz + 1));
  worldSetPoint(editor->world, 0);
  worldGetChunk(editor->world, input, sz);
  worldCloseBuffer(editor->world);
  input[sz] = '\0';
  worldSetCurrentBuffer(editor->world, prevBuffer, 0);
  uiActivateMiniWindow(editor->ui, 1);
  activeWnd = uiGetActiveWindow(editor->ui);
  uiSetWindowBufferName(editor->ui, activeWnd, "*messages*");
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
}

void editorChooseWindowBuffer(Editor *editor, char *bufferName)
{
  Window *activeWnd;
  activeWnd = uiGetActiveWindow(editor->ui);
  assert(bufferName != NULL);
  if (!worldBufferExists(editor->world, bufferName)) {
    worldCreateBuffer(editor->world, bufferName);
  }
  uiSetWindowBufferName(editor->ui, activeWnd, bufferName);
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

void editorSplitWindowVert(Editor *editor)
{
  uiSplitWindowVert(editor->ui);
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
  char *activeWndBufName;
  activeWndBufName = uiGetWindowBufferName(editor->ui,
                                           uiGetActiveWindow(editor->ui));
  uiNextWindow(editor->ui);
  worldSetCurrentBuffer(editor->world, activeWndBufName, 0);
  worldNotifyObservers(editor->world);
}

void editorPrevWindow(Editor *editor)
{
  char *activeWndBufName;
  activeWndBufName = uiGetWindowBufferName(editor->ui,
                                           uiGetActiveWindow(editor->ui));
  uiPrevWindow(editor->ui);
  worldSetCurrentBuffer(editor->world, activeWndBufName, 0);
  worldNotifyObservers(editor->world);
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
  Window *activeWnd;
  activeWnd = uiGetActiveWindow(editor->ui);
  worldNextBuffer(editor->world);
  uiSetWindowBufferName(editor->ui, activeWnd, worldGetBufferName(editor->world));
  worldNotifyObservers(editor->world);
}

void editorPrevBuffer(Editor *editor)
{
  Window *activeWnd;
  activeWnd = uiGetActiveWindow(editor->ui);
  worldPrevBuffer(editor->world);
  uiSetWindowBufferName(editor->ui, activeWnd, worldGetBufferName(editor->world));
  worldNotifyObservers(editor->world);
}

void editorCloseCurrentBuffer(Editor *editor)
{
  Window *bufferWnd;
  char *bufNameClose, *bufNameSwitch;

  bufNameClose = worldGetBufferName(editor->world);
  worldNextBuffer(editor->world);
  if (!strcmp(bufNameClose, worldGetBufferName(editor->world))) {
    editorShowMessage(editor, "The last buffer cannot be closed.", 1);
    return;
  }

  /* FIXME: This is not cool, setting the create flag below shouldn't be. */
  /* necessary. This may obfuscate a real problem. */
  worldSetCurrentBuffer(editor->world, bufNameClose, 1);
  worldCloseBuffer(editor->world);

  if (!strcmp(bufNameClose, "*messages*")) {
    worldCreateBuffer(editor->world, "*messages*");
  }

  bufferWnd = uiFindWindow(editor->ui, bufNameClose);
  if (bufferWnd) {
    bufNameSwitch = worldGetBufferName(editor->world);
    editorShowMessage(editor, "Buffer \"", 0);
    editorShowMessage(editor, bufNameClose, 0);
    editorShowMessage(editor, "\" closed", 1);
    uiSetWindowBufferName(editor->ui, bufferWnd, bufNameSwitch);
  }
  worldNotifyObservers(editor->world);
}

void editorSetPoint(Editor *editor, Position position)
{
  worldSetPoint(editor->world, position);
}

/* FIXME: Must use WorldCmds */
void editorPointToEndOfLine(Editor *editor)
{
  MoveBufferPointFn moveBufferPointFn;
  MoveBufferPointFn moveLineBufferPointFn;
  Mode *mode;
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);

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
  Window *currWnd;
  char *currBuffer;

  currBuffer = worldGetBufferName(editor->world);
  currWnd = uiFindWindow(editor->ui, currBuffer);
  mode = windowGetMode(currWnd);

  moveBufferPointFn = modeGetBufferPointFn(mode);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);

  moveLineBufferPointFn(editor->world, -1);
  moveLineBufferPointFn(editor->world, 1);

  worldNotifyObservers(editor->world);
}

void editorWriteBuffer(Editor *editor)
{
  char *buffer_name;
  buffer_name = worldGetBufferName(editor->world);

  if (!strlen(worldGetBufferFilePath(editor->world))) {
    worldSetBufferFilePath(editor->world, buffer_name);
  }

  worldWriteBuffer(editor->world);

  if (!(worldGetBufferFlags(editor->world) & WORLD_BUFFER_FLAG_MODIFIED)) {
    editorShowMessage(editor, "Buffer \"", 0);
    editorShowMessage(editor, buffer_name, 0);
    editorShowMessage(editor, "\" saved", 1);
  } else {
    editorShowMessage(editor, "Buffer \"", 0);
    editorShowMessage(editor, worldGetBufferName(editor->world), 0);
    editorShowMessage(editor, "\" could not be saved", 1);
  }
}

char *fileName(char *fpath)
{
  char *fname[2];

  fname[1] = fname[0] = fpath;
  while (*fpath != '\0') {
    if (*fpath == '/') {
      fname[0] = fname[1];
      fname[1] = fpath;
    }
    fpath += 1;
  }

  if (fname[1][1] == '\0')
    /* Probably a directory path ending with '/' */
    return fname[0] + ((fname[0][0] == '/') ? 1 : 0);

  return fname[1] + ((fname[1][0] == '/') ? 1 : 0);
}

void editorOpenFile(Editor *editor, char *filePath)
{
  char *bufferName;
  Window *activeWnd;
  activeWnd = uiGetActiveWindow(editor->ui);
  bufferName = fileName(filePath);
  worldCreateBuffer(editor->world, bufferName);
  worldSetCurrentBuffer(editor->world, bufferName, 1);
  worldSetBufferFilePath(editor->world, filePath);

  worldReadBuffer(editor->world);
  if ((worldGetBufferFlags(editor->world) & WORLD_BUFFER_FLAG_READ_ERROR)) {
    editorShowMessage(editor, "File \"", 0);
    editorShowMessage(editor, worldGetBufferFilePath(editor->world), 0);
    editorShowMessage(editor, "\" could not be read", 1);
  }

  uiSetWindowBufferName(editor->ui, activeWnd, bufferName);
  worldNotifyObservers(editor->world);
}

char *editorGetBufferFilePath(Editor *editor)
{
  return worldGetBufferFilePath(editor->world);
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

Size editorBufferSize(Editor *editor)
{
  return worldBufferSize(editor->world);
}

int editorGetBufferReadOnly(Editor *editor)
{
  return worldGetBufferFlags(editor->world) & WORLD_BUFFER_FLAG_RDONLY;
}

void editorSetBufferReadOnly(Editor *editor, int state)
{
  worldSetBufferFlag(editor->world, WORLD_BUFFER_FLAG_RDONLY, state);
}

Size
editorFetchBufferData(Editor *editor, Byte *dest, Position beg, Position end)
{
  Size nbytes;
  nbytes = ((end > beg) ? (end - beg) : (beg - end));
  worldAddMark(editor->world, "*_ed_fetch_bytes_*");
  if (nbytes) {
    worldSetPoint(editor->world, (beg < end) ? beg : end);
    nbytes = worldGetChunk(editor->world, dest, nbytes);
  }
  worldMarkToPoint(editor->world, "*_ed_fetch_bytes_*");
  worldRemoveMark(editor->world, "*_ed_fetch_bytes_*");
  return nbytes;
}

void editorCancel(Editor *editor)
{
  editorRecoverFromPromptedInput(editor);
}

void editorRedrawEntireScreen(Editor *editor)
{
  uiForceCleanupOnNextRedisplay(editor->ui);
  uiRedisplay(editor->ui, editor->world);
}

void editorAddExtension(Editor *editor, EditorExtension *extension)
{
  Hashed *hashed;
  hashed = hash_string(lsnew(Hashed), (char *)(extension->key));
  hash_table_put(editor->extensions, hashed, extension);
}

EditorExtension *editorGetExtension(Editor *editor, char *key)
{
  Hashed hashed;
  hash_string(&hashed, key);
  EditorExtension * match =
    (EditorExtension *)hash_table_get(editor->extensions, &hashed);

  hashed_fini(&hashed);
  return match;
}

void editorRemoveExtension(Editor *editor, char *key)
{
  Hashed hashed;
  hash_string(&hashed, key);
  hash_table_remove(editor->extensions, &hashed);
  hashed_fini(&hashed);
}


EditorExtension *createEditorExtension(const char *key, void *impl)
{
  EditorExtension *editorExt =
    (EditorExtension *)malloc(sizeof(EditorExtension));
  editorExt->key = strcpy((char *)malloc(sizeof(char) * (strlen(key) + 1)), key);
  editorExt->impl = impl;
  return editorExt;
}

void destroyEditorExtension(EditorExtension *editorExt)
{
 /* FIXME: This generates a warning */
 /*  free(editorExt->key); */
}

const char *editorExtensionGetKey(EditorExtension *editorExt)
{
  return editorExt->key;
}

void *editorExtensionGetImpl(EditorExtension *editorExt)
{
  return editorExt->impl;
}
