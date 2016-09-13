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

#include <diffexui/diffex.h>
#include "diffex-fifo.h"
#include "redisplay-utils.h"
#include <diffex-ui.h>

#include <uicore/ui.h>
#include <uicore/ui-private.h>
#include <uicore/terminal.h>

#include <string.h>

#include <libsys/dbglog.h>

void
deWindow_(Diffex *diffex, UI * ui, Window *window, World *world);
void deWindowManager_(Diffex *diffex, UI *ui, World *world);
void deCentralText_(Diffex *diffex, UI *ui, World *world);
void
deBuffer_(Diffex *diffex, World *world, Window *window, Terminal *terminal);
void deMiniWindow_(Diffex *diffex, UI *ui, World *world);
void deStatusLine_(Diffex *diffex, UI *ui, Window *window, World *world);

void diffexView(Diffex *diffex, UI *ui, World *world)
{
  Window *activeWindow;
  Terminal *activeTerminal;
  CALL(deWindowManager_, (diffex, ui, world));
  CALL(deCentralText_, (diffex, ui, world));
  CALL(deMiniWindow_, (diffex, ui, world));
  activeWindow = uiGetWindow(ui);
  activeTerminal = uiGetWindowTerminalById_(ui, windowGetId(activeWindow));
  terminalGrabFocus(activeTerminal);
  DBGLOG("\n", 0);
  deFifoSwap(diffex);
}

void deWindowManager_(Diffex *diffex, UI *ui, World *world)
{
  unsigned i, wndCount, oldWndCount;
  WindowManager *windowManager;
  Window *window, *activeWindow;

  windowManager = uiGetWindowManager_(ui);
  wndCount = windowManagerGetWindowCount(windowManager);
  oldWndCount = 0;

  activeWindow = window = windowManagerGetWindow(windowManager);
  deFifoPopPushUInt(diffex, &oldWndCount, &wndCount);
  FOR (i = 1, i <= wndCount, i++)
    window = windowManagerGetWindowById(windowManager, i);
    IF (window != NULL)
      CALL(deWindow_, (diffex, ui, window, world));
    END;
    if (window == NULL) {
      wndCount += 1;
    }
  END;

  if (diffex->mode != DE_ERASE) {
    terminalGrabFocus(uiGetWindowTerminalById_(ui, windowGetId(activeWindow)));
  }
}


void
deWindow_(Diffex *diffex, UI *ui, Window *window, World *world)
{
  unsigned posX, posY, sizeY, sizeX;
  unsigned oldPosY, oldPosX;
  unsigned oldSizeY, oldSizeX;
  unsigned winId;
  Terminal *terminal;
  short mustFreeTerminal;

  /* There may be special windows whose lifecycle
     is not controlled by the UI (miniWindow). */
  if ((diffex->mode != DE_ERASE) || (window != NULL)) {
    windowGetPosition(window, &posX, &posY);
    windowGetSize(window, &sizeX, &sizeY);
    winId = windowGetId(window);
    terminal = uiGetWindowTerminalById_(ui, winId);
    mustFreeTerminal = 0;
  }
  else {
    terminal = terminalFactoryCreate(ui->termFactory);
    mustFreeTerminal = 1;
  }
  deFifoPopPushUInt(diffex, &oldPosX, &posX);
  deFifoPopPushUInt(diffex, &oldPosY, &posY);
  deFifoPopPushUInt(diffex, &oldSizeX, &sizeX);
  deFifoPopPushUInt(diffex, &oldSizeY, &sizeY);

  switch (diffex->mode) {
  case DE_ERASE:
    ActualUIPrepareTerminal(ui->actualUi, ui, oldPosX, oldPosY,
                            oldSizeX, oldSizeY, terminal);
    ActualUIEraseBox(ui->actualUi, ui, terminal);
    ActualUIEraseTerminal(ui->actualUi, ui, terminal);
    destroyTerminal(terminal);
    CALL(deBuffer_, (diffex, world, window, terminal));
    IF (1)  /* Needed to clean fifo's registry */
      CALL(deStatusLine_, (diffex, ui, window, world));
    END
    break;
  case DE_SHOW:
    ActualUIPrepareTerminal(ui->actualUi, ui, posX, posY,
                            sizeX, sizeY, terminal);
    terminalClear(terminal);
    ActualUIDrawTerminal(ui->actualUi, ui, terminal);
    ActualUIDrawBox(ui->actualUi, ui, terminal);
    CALL(deBuffer_, (diffex, world, window, terminal));

    IF (windowHasStatusLine(window))
      CALL(deStatusLine_, (diffex, ui, window, world));
    END
    break;
  case DE_UPDATE:
    if ((sizeX != oldSizeX) || (sizeY != oldSizeY)
        || (posX != oldPosX) || (posY != oldPosY)) {
      ActualUIPrepareTerminal(ui->actualUi, ui, oldPosX, oldPosY,
                              oldSizeX, oldSizeY, terminal);
      ActualUIEraseTerminal(ui->actualUi, ui, terminal);
      ActualUIPrepareTerminal(ui->actualUi, ui, posX, posY,
                              sizeX, sizeY, terminal);
      ActualUIEraseBox(ui->actualUi, ui, terminal);
      ActualUIDrawTerminal(ui->actualUi, ui, terminal);
      ActualUIDrawBox(ui->actualUi, ui, terminal);

      diffex->mode = DE_ERASE;
      CALL(deBuffer_, (diffex, world, window, terminal));
      diffex->mode = DE_SHOW;
      CALL(deBuffer_, (diffex, world, window, terminal));
      diffex->mode = DE_UPDATE;
    }
    else {
      CALL(deBuffer_, (diffex, world, window, terminal));
    }

    IF (windowHasStatusLine(window))
      CALL(deStatusLine_, (diffex, ui, window, world));
    END
    break;
  }
  if (mustFreeTerminal) {
    destroyTerminal(terminal);
    free(terminal);
  }
}

void deCentralText_(Diffex *diffex, UI *ui, World *world)
{
  unsigned size, oldSize, textMax, oldTextMax;
  char *text, *oldText;

  textMax =
    ActualUIGetCenteredTextMax(ui->actualUi, ui);

  text = NULL;
  oldText = NULL;
  if (diffex->mode != DE_SHOW) {
    oldText =                   /* +1 for safety in case of an empty string */
      (char *)malloc(sizeof(char) * (oldTextMax * MULTIBYTE_CHAR_MAX_SIZE + 1));
  }

  if (diffex->mode != DE_ERASE) {
    uiGetCenteredMessage_(ui, &text, &size);
  }

  deFifoPopPushUInt(diffex, &oldTextMax, &textMax);
  deFifoPopPushStrN(diffex, oldText, text, &oldSize, &size);

  switch (diffex->mode) {
  case DE_ERASE:
    ActualUIEraseCenteredText(ui->actualUi, ui, oldText, oldSize);
    break;
  case DE_SHOW:
    ActualUIDrawCenteredText(ui->actualUi, ui, text, size);
    break;
  case DE_UPDATE:
    if (size != oldSize) {
      ActualUIEraseCenteredText(ui->actualUi, ui, oldText, oldSize);
      ActualUIDrawCenteredText(ui->actualUi, ui, text, size);
    }
    else if (strncmp(oldText, text, size)) {
      ActualUIEraseCenteredText(ui->actualUi, ui, oldText, oldSize);
      ActualUIDrawCenteredText(ui->actualUi, ui, text, size);
    }
    break;
  }

  if (text != NULL) {
    free(text);
  }
  if (oldText != NULL) {
    free(oldText);
  }
}

void deStatusLine_(Diffex *diffex, UI *ui, Window *window, World *world)
{
  unsigned size, oldSize;
  unsigned posX, posY, oldPosX, oldPosY;
  unsigned sizeX, sizeY, oldSizeX, oldSizeY;
  char *text, *oldText;

  text = NULL;
  oldText = NULL;
  if (diffex->mode != DE_ERASE) {
    windowGetPosition(window, &posX, &posY);
    windowGetSize(window, &sizeX, &sizeY);
    uiBuildStatusLine_(ui, window, &text, &size);
  }

  deFifoPopPushUInt(diffex, &oldSize, &size);

  if (diffex->mode != DE_SHOW) {
    oldText =
      (char *)malloc(sizeof(char) * (oldSize + 1));
  }

  deFifoPopPushUInt(diffex, &oldPosX, &posX);
  deFifoPopPushUInt(diffex, &oldPosY, &posY);
  deFifoPopPushUInt(diffex, &oldSizeX, &sizeX);
  deFifoPopPushUInt(diffex, &oldSizeY, &sizeY);
  deFifoPopPushStrN(diffex, oldText, text, &oldSize, &size);

  switch (diffex->mode) {
  case DE_ERASE:
    ActualUIEraseStatusLine(ui->actualUi, ui, oldPosX,
                            oldPosY, oldSizeX, oldSizeY,
                            oldText, oldSize);
    break;
  case DE_SHOW:
    ActualUIDrawStatusLine(ui->actualUi, ui, posX, posY,
                           sizeX, sizeY, text, size);
    break;
  case DE_UPDATE:
    if ((size != oldSize) || (posX != oldPosX) || (posY != oldPosY)
        || (sizeX != oldSizeX) || (sizeY != oldSizeY)) {
      ActualUIEraseStatusLine(ui->actualUi, ui, oldPosX,
                              oldPosY, oldSizeX, oldSizeY,
                              oldText, oldSize);
      ActualUIDrawStatusLine(ui->actualUi, ui, posX, posY,
                             sizeX, sizeY, text, size);
    }
    else if (strncmp(oldText, text, size)) {
      ActualUIEraseStatusLine(ui->actualUi, ui, oldPosX,
                              oldPosY, oldSizeX, oldSizeY,
                              oldText, oldSize);
      ActualUIDrawStatusLine(ui->actualUi, ui, posX, posY,
                             sizeX, sizeY, text, size);
    }
    break;
  }

  if (oldText != NULL) {
    free(oldText);
  }
  if (text != NULL) {
    free(text);
  }
}

void deMiniWindow_(Diffex *diffex, UI *ui, World *world)
{
  Terminal *terminal;
  terminal = uiGetWindowTerminalById_(ui, MINI_WINDOW_ID);
  /* CALL(deBuffer_,(diffex, world, ui->miniWindow, terminal)); */
  CALL(deWindow_, (diffex, ui, ui->miniWindow, world));
}

void deBuffer_(Diffex *diffex, World *world, Window *window, Terminal *terminal)
{
  Size sz;
  unsigned termHeight, oldTermHeight;
  unsigned termWidth, oldTermWidth;
  MoveBufferPointFn movePoint, moveLine;
  unsigned row;
  unsigned col;
  unsigned pointRow;
  unsigned pointCol;
  char *line;
  char *oldLine;
  char c[MULTIBYTE_CHAR_MAX_SIZE];
  unsigned lineSz, oldLineSz;
  unsigned lineMax, oldLineMax;
  unsigned lineCols, oldLineCols;
  Mode *mode;
  short cursorSaved;
  TextWalker walker;
  Position startPoint, colSearchPoint;

  pointRow = pointCol = 0;
  cursorSaved = 0;
  line = NULL;
  lineMax = 0;
  startPoint = 0;
  colSearchPoint = 0;

  if (diffex->mode == DE_ERASE) {
    terminalClear(terminal);
  }
  else {
    termHeight = terminalGetHeight(terminal);
    termWidth = terminalGetWidth(terminal);
    lineMax = MULTIBYTE_CHAR_MAX_SIZE * termWidth;
    line = (char *)malloc(sizeof(char) * lineMax);

    worldSetCurrentBuffer(world, windowGetBufferName(window), 0);
    startPoint = worldGetPoint(world);
    worldAddMark(world, "start");
    framer(window, terminal, world);
    worldSetPoint(world, windowGetTop(window));

    mode = windowGetMode(window);
    movePoint = modeGetBufferPointFn(mode);
    moveLine = modeGetBufferPointLineFn(mode);
  }

  oldTermWidth = 0;
  oldTermHeight = 0;
  deFifoPopPushUInt(diffex, &oldTermHeight, &termHeight);
  deFifoPopPushUInt(diffex, &oldTermWidth, &termWidth);

  oldLine = NULL;
  oldLineMax = 0;
  if (diffex->mode != DE_SHOW) {
    oldLineMax = MULTIBYTE_CHAR_MAX_SIZE * oldTermWidth;
    oldLine =
      (char *)malloc(sizeof(char) * oldLineMax);
  }

  if (diffex->mode == DE_ERASE) {
    termHeight = oldTermHeight;
    termWidth = oldTermWidth;
  }

  char *curr, *old;
  unsigned currSz, oldSz, diff1Sz, diff2Sz;
  Position pointBkpCurrLine, pointBkpNextLine;
  FOR (row = 0, row < termHeight, row++)
    lineSz = 0;
    if (diffex->mode != DE_ERASE) {
      pointBkpCurrLine = worldGetPoint(world);
      fetchBytes(world, moveLine, line, &lineSz);
    }

    oldLineSz = 0;
    deFifoPopPushStrN(diffex, oldLine, line, &oldLineSz, &lineSz);

    old = oldLine;
    oldSz = oldLineSz;
    curr = line;
    currSz = lineSz;
    DBGLOG("LINE(%d): (%.*s)\n", lineSz, lineSz, line);
    if ((diffex->mode != DE_ERASE) && (lineSz || oldLineSz)) {
      pointBkpNextLine = worldGetPoint(world);
      worldSetPoint(world, pointBkpCurrLine);

      if (pointBkpCurrLine <= startPoint
          && startPoint <= pointBkpNextLine) {
        pointRow = row;
        colSearchPoint = pointBkpCurrLine;
      }

      initTextWalker(&walker, world, movePoint);
      col =
        textDiff(&curr, &old, &currSz, &oldSz, &diff1Sz, &diff2Sz, &walker);
      while (currSz || oldSz) {
        unsigned writeSz;
        writeSz = diff1Sz - ((curr[diff1Sz - 1] == '\n') ? 1 : 0);
        if (diff2Sz) {
          terminalSetCursor(terminal, col, row);
          terminalDelete(terminal, diff2Sz);
        }
        if (writeSz) {
          terminalSetCursor(terminal, col, row);
          terminalWrite(terminal, curr, writeSz);
        }
        col +=
          textNextDiff(&curr, &old, &currSz, &oldSz, &diff1Sz, &diff2Sz, &walker);
      }
      destroyTextWalker(&walker);
      worldSetPoint(world, pointBkpNextLine);
    }
  END;

  if (diffex->mode != DE_ERASE) {
    int i, nbytes;
    char c[MULTIBYTE_CHAR_MAX_SIZE];
    i = 0;
    worldSetPoint(world, colSearchPoint);
    fetchBytes(world, movePoint, c, &nbytes);
    for (pointCol = 0; colSearchPoint < startPoint; colSearchPoint++) {
      if (++i == nbytes) {
        pointCol += glyphSizeOfSingleChar_(c, nbytes);
        fetchBytes(world, movePoint, c, &nbytes);
        i = 0;
      }
    }

    worldMarkToPoint(world, "start");
    worldRemoveMark(world, "start");
    DBGLOG("CURSOR: (%d, %d)\n", pointCol, pointRow);
    terminalSetCursor(terminal, pointCol, pointRow);
    terminalFlush(terminal);
  }

  if (line != NULL) free(line);
  if (oldLine != NULL) free(oldLine);
}
