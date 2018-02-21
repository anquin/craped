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

#include <libsys/dbglog.h>

#include <stdlib.h>

#include <uicore/ui.h>
#include <uicore/buffer_point_moves.h>
#include <editor.h>
#include <editorcmd.h>

#include "craped.h"
#include "setup.h"
#include "setup-ui.h"
#include "craped-editor-subscriber.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

struct craped
{
  Editor *editor;
  EditorSetup *edsetup;
  EditorExtension *editorExt;
};

const char CRAPED_EXT_KEY[] = "craped";

Craped *createCraped(int argc, char *argv[], CrapedSubscriber **subscribers)
{
  Craped *craped;
  EditorSetup *edsetup;
  Editor *editor;
  Mode *mode;
  EditorSubscriber **editorSubscribers;
  CrapedEditorSubscriber *crapedEditorSubscriber;
  unsigned subsLen, i;

  DBGLOG_OPEN(fopen("/tmp/crapeddbg.log", "wb"));

  craped = (Craped *)malloc(sizeof(Craped));

  edsetup = (EditorSetup *)malloc(sizeof(EditorSetup));
  setup(edsetup, argc, argv, PACKAGE_STRING);

  subsLen = 10; i = 0;
  editorSubscribers =
    (EditorSubscriber **)malloc(sizeof(EditorSubscriber *) * subsLen);
  while (*subscribers != NULL) {
    crapedEditorSubscriber =
      createCrapedEditorSubscriber(*(subscribers++), craped);
    editorSubscribers[i++] =
      createEditorSubscriber(crapedEditorSubscriber, crapedEditorSubscriberFn);
    if (i >= (subsLen - 1)) {
      subsLen *= 2;
      editorSubscribers =
        (EditorSubscriber **)realloc(editorSubscribers,
                                     sizeof(EditorSubscriber *) * subsLen);
    }
  }
  editorSubscribers[i] = NULL;

  mode = (Mode *)malloc(sizeof(Mode));
  initMode(mode, utf8MovePoint, utf8WordMovePoint, utf8LineMovePoint);
  windowSetMode(uiGetActiveWindow(edsetup->ui), mode);

  editor = createEditor(edsetup->ui, editorSubscribers, edsetup->progName);
  craped->editor = editor;
  craped->edsetup = edsetup;

  EditorExtension *extension = createEditorExtension(CRAPED_EXT_KEY, craped);
  editorAddExtension(editor, extension);
  craped->editorExt = extension;

  for (i = 0; i < edsetup->nFilesToOpen; i++) {
    crapedOpenFile(craped, edsetup->filesToOpen[i]);
  }

  return craped;
}

void destroyCraped(Craped *craped)
{
  destroyEditor(craped->editor);
  free(craped->editor);
  cleanup(craped->edsetup);
  free(craped->edsetup);
  destroyEditorExtension(craped->editorExt);
  free(craped->editorExt);

  DBGLOG_CLOSE();
}


void crapedRun(Craped *craped)
{
  editorRun(craped->editor);
}

void crapedExit(Craped *craped)
{
  editorExit(craped->editor);
}

void crapedInsert(Craped *craped, Byte *info, Size size)
{
  editorInsert(craped->editor, info, size);
}

void crapedBackspace(Craped *craped)
{
  editorBackspace(craped->editor);
}

void crapedDelete(Craped *craped, Size size)
{
  editorDelete(craped->editor, size);
}

void crapedLineFeed(Craped *craped)
{
  editorLineFeed(craped->editor);
}

void crapedInsertTab(Craped *craped)
{
  editorInsertTab(craped->editor);
}

void crapedMoveLeft(Craped *craped)
{
  editorMoveLeft(craped->editor);
}

void crapedMoveRight(Craped *craped)
{
  editorMoveRight(craped->editor);
}

void crapedMoveUp(Craped *craped)
{
  editorMoveUp(craped->editor);
}

void crapedMoveDown(Craped *craped)
{
  editorMoveDown(craped->editor);
}

void crapedPrevWord(Craped *craped)
{
  editorPrevWord(craped->editor);
}

void crapedNextWord(Craped *craped)
{
  editorNextWord(craped->editor);
}

void crapedConnectToSharing(Craped *craped, char *host, int port)
{
  editorConnectToSharing(craped->editor, host, port);
}

void crapedPromptForInput(Craped *craped, char *editorCmdStr, char *label)
{
  editorPromptForInput(craped->editor, editorCmdStr, label);
}

char *crapedRecoverFromPromptedInput(Craped *craped)
{
  return editorRecoverFromPromptedInput(craped->editor);
}

void crapedChooseBuffer(Craped *craped, char *bufferName)
{
  editorChooseBuffer(craped->editor, bufferName);
}

void crapedChooseWindowBuffer(Craped *craped, char *buffername)
{
  editorChooseWindowBuffer(craped->editor, buffername);
}

void crapedSplitWindowHorz(Craped *craped)
{
  editorSplitWindowHorz(craped->editor);

}

void crapedSplitWindowVert(Craped *craped)
{
  editorSplitWindowVert(craped->editor);
}

void crapedRemoveCurrentWindow(Craped *craped)
{
  editorRemoveCurrentWindow(craped->editor);
}

void crapedNextWindow(Craped *craped)
{
  editorNextWindow(craped->editor);
}

void crapedPrevWindow(Craped *craped)
{
  editorPrevWindow(craped->editor);
}

void crapedInitSharing(Craped *craped)
{
  editorInitSharing(craped->editor);
}

void crapedStopSharing(Craped *craped)
{
  editorStopSharing(craped->editor);
}

void crapedSetSharingPort(Craped *craped, int port)
{
  editorSetSharingPort(craped->editor, port);
}

void crapedNextBuffer(Craped *craped)
{
  editorNextBuffer(craped->editor);
}

void crapedPrevBuffer(Craped *craped)
{
  editorPrevBuffer(craped->editor);
}

void crapedSplitWindow(Craped *craped)
{
  /* editorSplitWindow(craped->editor); */
}

void crapedCloseCurrentBuffer(Craped *craped)
{
  editorCloseCurrentBuffer(craped->editor);
}

void crapedSetPoint(Craped *craped, Position position)
{
  editorSetPoint(craped->editor, position);
}

void crapedPointToEndOfLine(Craped *craped)
{
  editorPointToEndOfLine(craped->editor);
}

void crapedPointToLineBegin(Craped *craped)
{
  editorPointToLineBegin(craped->editor);
}

void crapedWriteBuffer(Craped *craped)
{
  editorWriteBuffer(craped->editor);
}

void crapedOpenFile(Craped *craped, char *filePath)
{
  editorOpenFile(craped->editor, filePath);
}

char *crapedGetBufferFilePath(Craped *craped)
{
  return editorGetBufferFilePath(craped->editor);
}

void crapedSetBufferFilePath(Craped *craped, char *filePath)
{
  editorSetBufferFilePath(craped->editor, filePath);
}

void crapedShareBuffer(Craped *craped, int share)
{
  editorShareBuffer(craped->editor, share);
}

int crapedIsBufferShared(Craped *craped)
{
  return editorIsBufferShared(craped->editor);
}

Size crapedBufferSize(Craped *craped)
{
  return editorBufferSize(craped->editor);
}

Size
crapedFetchBufferData(Craped *craped, Byte *dest, Position beg, Position end)
{
  return editorFetchBufferData(craped->editor, dest, beg, end);
}

void crapedRegisterCommand(Craped *craped, char *cmdStr, CrapedCmdFn fn)
{
  editorRegisterExtensionCommand(craped->editor, CRAPED_EXT_KEY,
                                 cmdStr, (void (*)(void *, void*))fn);
}

void crapedBindKeyCombo(Craped *craped, char *keyCombo, char *cmdStr)
{
  editorBindKeyCombo(craped->editor, keyCombo, cmdStr);
}

void crapedShowMessage(Craped *craped, char *msg, short lf)
{
  editorShowMessage(craped->editor, msg, lf);
}

void crapedCancel(Craped *craped)
{
  editorCancel(craped->editor);
}

int crapedCmdGetParamSz(CrapedCmd *crapedCmd)
{
  return ((EditorCmd *)crapedCmd)->paramSz;
}

void crapedCmdSetParamSz(CrapedCmd *crapedCmd, int paramSz)
{
  ((EditorCmd *)crapedCmd)->paramSz = paramSz;
}

char *crapedCmdGetParam(CrapedCmd *crapedCmd)
{
  return ((EditorCmd *)crapedCmd)->param;
}

void crapedCmdSetParam(CrapedCmd *crapedCmd, char *param)
{
  ((EditorCmd *)crapedCmd)->param = param;
}
