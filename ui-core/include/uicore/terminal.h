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

#ifndef TERMINAL_H
#define TERMINAL_H

#include "keys.h"
#include <libsys/def.h>

struct terminal;
typedef struct terminal Terminal;

void destroyTerminal(Terminal *);
void terminalClear(Terminal *);
void terminalGetCursor(Terminal *, unsigned *, unsigned *);
void terminalSetCursor(Terminal *, unsigned, unsigned);
void terminalClearEndOfLine(Terminal *);
void terminalWrite(Terminal *, Byte *, Size);
void terminalDelete(Terminal *, Size);
void terminalOutBlanks(Terminal *, Size);
unsigned terminalGetWidth(Terminal *);
unsigned terminalGetHeight(Terminal *);
void terminalGetDimensions(Terminal *, unsigned *w, unsigned *h);
void terminalFlush(Terminal *);
KbInput *terminalGetInput(Terminal *);
#include "box.h"
void
terminalDrawBox(Terminal *, struct box *box);
void
terminalEraseBox(Terminal *, struct box *box);
void terminalGrabFocus(Terminal *);

#endif
