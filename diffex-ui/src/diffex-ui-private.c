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

#include <diffexui/diffex-ui-private.h>
#include "diffex-ui.h"

#include <stdlib.h>
#include <string.h>

#include <libsys/dbglog.h>

Terminal *uiGetWindowTerminalById_(UI *ui, unsigned winId)
{
  if (winId == MINI_WINDOW_ID) {
    return ui->miniTerminal;
  }
  return ui->terminals[winId];
}

WindowManager *uiGetWindowManager_(UI *ui)
{
  return ui->windowManager;
}

void uiGetCenteredMessage_(UI *ui, char **message, unsigned *size)
{
  *size = ui->centeredMessageSize;
  if (ui->centeredMessageSize) {
    *message = (char *)malloc(sizeof(char) * ui->centeredMessageSize + 1);
    memcpy(*message, ui->centeredMessage, ui->centeredMessageSize);
  }
}

void uiBuildStatusLine_(UI *ui, Window *window, char **line, unsigned *size)
/* FIXME: This function currently assumes
   that every char is represented by 1 byte. */
{
  char *bufferName;
  Terminal *term;
  size_t bufNamLen;

  term = uiGetWindowTerminalById_(ui, windowGetId(window));
  *size = terminalGetWidth(term);

  *line = (char *)malloc(sizeof(char) * (*size + 1));
  memset(*line, '=', *size);
  (*line)[*size] = '\0';

  bufferName = windowGetBufferName(window);
  bufNamLen = strlen(bufferName);
  (*line)[1] = '[';
  memcpy((*line) + 2, bufferName, sizeof(char) * bufNamLen);
  (*line)[bufNamLen + 2] = ']';
}

/* TODO: remove */
/* Terminal *uiGetMiniTerminal_(UI *ui) */
/* { */
/*   return ui->miniTerminal; */
/* } */
