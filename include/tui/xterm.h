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

#ifndef XTERM_H
#define XTERM_H

#include <uicore/terminal.h>

#include <termios.h>

#define MODEMDEVICE "/dev/tty"

typedef struct terminal
{
  int fd;
  unsigned currX, currY;
  struct termios tio;
  struct termios old_tio;
  unsigned posX, posY;
  struct terminal *parent;

  /* Used if parent != NULL */
  unsigned height, width;
} XTerm;

Terminal *createXTerm(XTerm *parent, unsigned posX, unsigned posY);
Terminal *initXTerm(XTerm *, XTerm *parent, unsigned posX, unsigned posY);

#define xtermSetPosX(xt, px) (xt->posX = px)
#define xtermSetPosY(xt, py) (xt->posY = py)
#define xtermGetPosX(xt) (xt->posX)
#define xtermGetPosY(xt) (xt->posY)
#define xtermSetHeight(xt, h) (xt->height = h)
#define xtermSetWidth(xt, w) (xt->width = w)

#endif
