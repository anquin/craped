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

#ifndef EDITOR_H
#define EDITOR_H

#include <libsys/def.h>

#define DEFAULT_SHARING_PORT 4444
#define KEY_COMBO_MAX_SIZE 10

#define BUFFER_FLAG_SHARED 0x00010000

typedef struct editor Editor;

/* TODO: add encoding param */
#define editorKeyHandle uiObserverHandleInput

typedef struct editor_subscriber EditorSubscriber;
typedef void (*EditorSubscriberFn)(void *, Editor *);
EditorSubscriber *createEditorSubscriber(void *impl, EditorSubscriberFn fn);

/* "subscribers" is a NULL terminated array of subscribers */
Editor *createEditor(void *ui,
                     EditorSubscriber **subscribers,
                     const char *startupMessage);
/* "subscribers" is a NULL terminated array of subscribers */
void initEditor(Editor *,
                void *ui,
                EditorSubscriber **subscribers,
                const char *startupMessage);
void destroyEditor(Editor *);

void editorRun(Editor *editor);
void editorExit(Editor *editor);
void editorInsert(Editor *editor, Byte *info, Size size);
void editorBackspace(Editor *editor);
void editorDelete(Editor *editor, Size size);
void editorLineFeed(Editor *editor);
void editorInsertTab(Editor *editor);
void editorMoveLeft(Editor *editor);
void editorMoveRight(Editor *editor);
void editorMoveUp(Editor *editor);
void editorMoveDown(Editor *editor);
void editorPageUp(Editor *editor);
void editorPageDown(Editor *editor);
void editorPrevWord(Editor *editor);
void editorNextWord(Editor *editor);
void editorConnectToSharing(Editor *editor, char *host, int port);
void editorPromptForInput(Editor *editor, char *editorCmdStr, char *label);
char *editorRecoverFromPromptedInput(Editor *editor);
void editorChooseBuffer(Editor *editor, char *bufferName);
void editorChooseWindowBuffer(Editor *editor, char *buffername);
void editorSplitWindowHorz(Editor *editor);
void editorSplitWindowVert(Editor *editor);
void editorRemoveCurrentWindow(Editor *editor);
void editorNextWindow(Editor *editor);
void editorPrevWindow(Editor *editor);
void editorInitSharing(Editor *editor);
void editorStopSharing(Editor *editor);
void editorSetSharingPort(Editor *editor, int);
void editorNextBuffer(Editor *editor);
void editorPrevBuffer(Editor *editor);
void editorCloseCurrentBuffer(Editor *editor);
void editorSetPoint(Editor *editor, Position position);
void editorPointToEndOfLine(Editor *editor);
void editorPointToLineBegin(Editor *editor);
void editorWriteBuffer(Editor *editor);
void editorOpenFile(Editor *editor, char *filePath);
char *editorGetBufferFilePath(Editor *editor);
void editorSetBufferFilePath(Editor *editor, char *filePath);
/* shares current buffer if share != 0 */
void editorShareBuffer(Editor *editor, int share);
/* Returns non-zero if the current buffer is shared. */
int editorIsBufferShared(Editor *editor);
/* Returns buffer size in bytes */
Size editorBufferSize(Editor *editor);
int editorGetBufferReadOnly(Editor *editor);
void editorSetBufferReadOnly(Editor *editor, int state);
Size
editorFetchBufferData(Editor *editor, Byte *dest, Position beg, Position end);
void editorRegisterCommand(Editor *editor, char *cmdStr, void *fn);
void editorRegisterExtensionCommand(Editor *editor, const char *editorExtKey,
                                    char *cmdStr, void *fn);
void editorBindKeyCombo(Editor *editor, char *keyCombo, char *cmdStr);
void editorShowMessage(Editor *, char *, short lf);
void editorUndo(Editor *editor);
void editorCancel(Editor *editor);
void editorRedrawEntireScreen(Editor *editor);
typedef struct editor_extension EditorExtension;
void editorAddExtension(Editor *editor, EditorExtension *extension);
EditorExtension *editorGetExtension(Editor *editor, char *key);
void editorRemoveExtension(Editor *editor, char *key);

EditorExtension *createEditorExtension(const char *key, void *impl);
void destroyEditorExtension(EditorExtension *editorExt);
const char *editorExtensionGetKey(EditorExtension *editorExt);
void *editorExtensionGetImpl(EditorExtension *editorExt);

#endif
