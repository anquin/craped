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

#ifndef EDITORCMD_FUNCS_H
#define EDITORCMD_FUNCS_H

#include <editorcmd.h>

/* Default EditorCmd ExecuteFns */
void editorCmdFnExit(Editor *, EditorCmd *);
void editorCmdFnInsert(Editor *, EditorCmd *);
void editorCmdFnBackspace(Editor *, EditorCmd *);
void editorCmdFnDelete(Editor *, EditorCmd *);
void editorCmdFnLineFeed(Editor *, EditorCmd *);
void editorCmdFnInsertTab(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnMoveLeft(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnMoveRight(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnMoveUp(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnMoveDown(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPrevWord(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnNextWord(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPromptToInitSharing(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnInitSharing(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPromptToConnectToSharing(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnConnectToSharing(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnChooseBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnChooseWindowBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPromptToSetWindowBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnSplitWindowHorz(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnSplitWindowVert(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnRemoveCurrentWindow(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnNextWindow(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPrevWindow(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnNextBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPrevBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnSplitWindow(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnCloseCurrentBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPointToEndOfLine(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPointToLineBegin(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnWriteBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnOpenFile(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPromptToOpenFile(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnToggleReadOnly(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnToggleShareBuffer(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnPromptToBindKeyCombo(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnBindKeyCombo(Editor *editor, EditorCmd *editorCmd);
void editorCmdFnCancel(Editor *editor, EditorCmd *editorCmd);
/* ----------------- */

#endif
