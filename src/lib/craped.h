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

#ifndef CRAPED_H
#define CRAPED_H

#ifndef LIBSYS_DEF_H
#include <craped/def.h>
#endif

typedef struct craped Craped;
typedef void CrapedCmd;
typedef void (*CrapedCmdFn)(Craped *, CrapedCmd *);
int crapedCmdGetParamSz(CrapedCmd *crapedCmd);
void crapedCmdSetParamSz(CrapedCmd *crapedCmd, int paramSz);
char *crapedCmdGetParam(CrapedCmd *crapedCmd);
void crapedCmdSetParam(CrapedCmd *crapedCmd, char *param);

typedef void (*CrapedSubscriberFn)(void *, Craped *);
typedef struct craped_subscriber
{
  void *impl;
  CrapedSubscriberFn fn;
} CrapedSubscriber;
CrapedSubscriber *createCrapedSubscriber(void *impl, CrapedSubscriberFn fn);

/* "subscribers" is a NULL terminated array of subscribers */
Craped *createCraped(int argc, char *argv[], CrapedSubscriber **subscribers);
void destroyCraped(Craped *craped);

/* API */
void crapedRun(Craped *craped);
void crapedExit(Craped *craped);
void crapedInsert(Craped *craped, Byte *info, Size size);
void crapedBackspace(Craped *craped);
void crapedDelete(Craped *craped);
void crapedLineFeed(Craped *craped);
void crapedInsertTab(Craped *craped);
void crapedMoveLeft(Craped *craped);
void crapedMoveRight(Craped *craped);
void crapedMoveUp(Craped *craped);
void crapedMoveDown(Craped *craped);
void crapedPrevWord(Craped *craped);
void crapedNextWord(Craped *craped);
void crapedConnectToSharing(Craped *craped, char *host, int port);
void crapedPromptForInput(Craped *craped, char *editorCmdStr, char *label);
char *crapedRecoverFromPromptedInput(Craped *craped);
void crapedChooseBuffer(Craped *craped, char *bufferName);
void crapedChooseWindowBuffer(Craped *craped, char *buffername);
void crapedSplitWindowHorz(Craped *craped);
void crapedRemoveCurrentWindow(Craped *craped);
void crapedNextWindow(Craped *craped);
void crapedPrevWindow(Craped *craped);
void crapedInitSharing(Craped *craped);
void crapedStopSharing(Craped *craped);
void crapedSetSharingPort(Craped *craped, int);
void crapedNextBuffer(Craped *craped);
void crapedPrevBuffer(Craped *craped);
void crapedSplitWindow(Craped *craped);
void crapedCloseCurrentBuffer(Craped *craped);
void crapedSetPoint(Craped *craped, Position position);
void crapedPointToEndOfLine(Craped *craped);
void crapedPointToLineBegin(Craped *craped);
void crapedWriteBuffer(Craped *craped);
void crapedOpenFile(Craped *craped, char *filePath);
char *crapedGetBufferFilePath(Craped *craped);
void crapedSetBufferFilePath(Craped *craped, char *filePath);
/* shcrapedurrent buffer if share != 0 */
void crapedShareBuffer(Craped *craped, int share);
/* Returns non-zero if the current buffer is shared. */
int crapedIsBufferShared(Craped *craped);
Size crapedBufferSize(Craped *craped);
Size
crapedFetchBufferData(Craped *craped, Byte *dest, Position beg, Position end);
void crapedRegisterCommand(Craped *craped, char *cmdStr, CrapedCmdFn fn);
void crapedBindKeyCombo(Craped *craped, char *keyCombo, char *cmdStr);
void crapedShowMessage(Craped *, char *, short lf);
void crapedCancel(Craped *craped);
/* *** */

#endif
