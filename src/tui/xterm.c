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

#include <tui/xterm.h>
#include <uicore/input.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <assert.h>

#include <libsys/dbglog.h>

Terminal *createXTerm(XTerm *parent, unsigned posX, unsigned posY)
{
  XTerm *term;

  term = (XTerm *)malloc(sizeof(XTerm));
  initXTerm(term, parent, posX, posY);

  return term;
}

Terminal *initXTerm(XTerm *term, XTerm *parent, unsigned posX, unsigned posY)
{
  if (parent == NULL) {
    term->fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
    if (term->fd < 0) { fprintf(stderr, "lol\n"); exit(-1); }
    tcgetattr(term->fd, &(term->tio));
    term->old_tio = term->tio;
    term->tio.c_lflag &= ~(ICANON|ECHO|ISIG);
    term->tio.c_iflag &= ~(IXON|IXOFF);
    tcflush(term->fd, TCIFLUSH);
    tcsetattr(term->fd, TCSANOW, &(term->tio));
    setlocale(LC_CTYPE, "");
    term->parent = NULL;
    term->posX = term->posY = 0;
  }
  else {
    term->posX = posX;
    term->posY = posY;
    term->height = terminalGetHeight(parent);
    term->width = terminalGetWidth(parent);
    term->parent = parent;
    term->fd = parent->fd;
  }
}

void terminalClear(Terminal *term)
{
  unsigned y;
  unsigned w, h;

  if (term->parent == NULL) {
    printf("\e[1;1H\e[2J");
  }
  else {
    w = terminalGetWidth(term);
    h = terminalGetHeight(term);
    for (y = 0; y < h; y++) {
      terminalSetCursor(term, 0, y);
      terminalClearEndOfLine(term);
    }
  }
  terminalSetCursor(term, 0, 0);
}

void terminalSetCursor(Terminal *term, unsigned x, unsigned y)
{
  /* DBGLOG("terminalSetCursor(0x%x, %d, %d)\n", term, x, y); */

  term->currX = x;
  term->currY = y;

  if (term->parent == NULL) {
    printf("\x1b[%d;%dH", term->posY + y + 1, term->posX + x + 1);
  }

  else {
    unsigned actualX, actualY;
    actualX = xtermGetPosX(term) + x;
    actualY = xtermGetPosY(term) + y;
    terminalSetCursor(term->parent, actualX, actualY);
  }
}

void terminalGetCursor(Terminal *term, unsigned *x, unsigned *y)
{
  *x = term->currX;
  *y = term->currY;
}

#define TERMINAL_ENFORCE_CURSOR(term) terminalSetCursor(term, term->currX, term->currY)

void terminalClearEndOfLine(Terminal *term)
{
  unsigned col;

  DBGLOG("terminalCEOL()\n", 0);

  TERMINAL_ENFORCE_CURSOR(term);
  for (col = 0; col < terminalGetWidth(term); col++) {
    printf(" ");
  }
  TERMINAL_ENFORCE_CURSOR(term);
}

void treatOverflows_(Terminal *term)
{
  term->currX = xtermGetPosX(term);
  term->currY += 1;
  if (term->currY >= terminalGetHeight(term)) {
    /* TODO: scroll? error? */
  }
}

void terminalWrite(Terminal *term, Byte *info, Size size)
{
  int len;
  unsigned termWidth;
  Terminal *rootTerm;
  termWidth = terminalGetWidth(term);

  DBGLOG("terminalWrite(0\\x%x, %.*s, %d)\n", term, size, info, size);

  if (!size) {
    return;
  }

  /* It seems safer (but more costly) to call terminalSetCursor here
     than writing the cursor position computing code all over again */
  TERMINAL_ENFORCE_CURSOR(term);

  rootTerm = term;
  while (rootTerm->parent != NULL) {
    rootTerm = rootTerm->parent;
  }

  #ifdef DEBUG_MODE
  unsigned x, y;
  terminalGetCursor(rootTerm, &x, &y);
  DBGLOG("\t(x, y) = (%d, %d)\n", x, y);
  #endif

  do {
    len = mblen(info, size);
    fwrite(info, 1, len, stdout);
    size -= len;
    info += len;
    term->currX += 1;

    if (term->currX >= termWidth) {
      treatOverflows_(term);
    }
  } while (size && len > 0);

  TERMINAL_ENFORCE_CURSOR(term);
}

void terminalDelete(Terminal *term, Size size)
{
  DBGLOG("terminalDelete(0x%x, %d)\n", term, size);
  unsigned width, height;
  unsigned actualX, actualY;
  unsigned col, row;

  TERMINAL_ENFORCE_CURSOR(term);
  printf("\x1b[%dP", size);
}

void terminalOutBlanks(Terminal *term, Size size)
{
  Byte *blanks;
  blanks = (Byte *)malloc(sizeof(Byte) * size);
  memset(blanks, ' ', size);
  terminalWrite(term, blanks, size);
}

unsigned terminalGetWidth(Terminal *term)
{
  if (term->parent == NULL) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    return ws.ws_col;
  }
  return term->width;
}

unsigned terminalGetHeight(Terminal *term)
{
  if (term->parent == NULL) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    return ws.ws_row;
  }
  return term->height;
}

void terminalGetDimensions(Terminal *term, unsigned *w, unsigned *h)
{
  if (term->parent == NULL) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    *w = ws.ws_row;
    *h = ws.ws_col;
  } else {
    *w = term->width;
    *h = term->height;
  }
}

void terminalFlush(Terminal *term)
{
  fflush(stdout);
}

KbInput *terminalGetInput(Terminal *term)
{
  return getInput();
}

void
terminalDrawEraseBox(Terminal *term, struct box *box, short erase)
{
  unsigned bkpX, bkpY;
  unsigned currX, currY;
  unsigned x, y;
  static unsigned short vchar = 0, hchar = 1;
  static char fills[2][2] = {{"|-"}, {"  "}};

  terminalGetCursor(term, &bkpX, &bkpY);

  x = box->x;
  y = box->y;

  for (currY = y; currY <= y + box->h; currY++) {
    if (box->l) {
      terminalSetCursor(term, x, currY);
      terminalWrite(term, &fills[erase][vchar], 1);
    }
    if (box->r) {
      terminalSetCursor(term, x + box->w, currY);
      terminalWrite(term, &fills[erase][vchar], 1);
    }
  }
  for (currX = x; currX < x + box->w; currX++) {
    if (box->t) {
      terminalSetCursor(term, currX, y);
      terminalWrite(term, &fills[erase][hchar], 1);
    }
    if (box->b) {
      terminalSetCursor(term, currX, y + box->h);
      terminalWrite(term, &fills[erase][hchar], 1);
    }
  }

  terminalSetCursor(term, bkpX, bkpY);
}

void terminalDrawBox(Terminal *term, struct box *box)
{
  terminalDrawEraseBox(term, box, 0);
}

void terminalEraseBox(Terminal *term, struct box *box)
{
  terminalDrawEraseBox(term, box, 1);
}

void terminalGrabFocus(Terminal *terminal)
{
  TERMINAL_ENFORCE_CURSOR(terminal);
}

void destroyTerminal(Terminal *term)
{
  if (term->parent == NULL) {
    terminalClear(term);
    tcsetattr(term->fd, TCSANOW, &(term->old_tio));
  }
}
