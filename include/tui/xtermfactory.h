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

#ifndef XTERM_FACTORY
#define XTERM_FACTORY

#include <uicore/terminalfactory.h>
#include "xterm.h"

typedef struct terminal_factory
{
  XTerm *rootTerm;
} XtermTerminalFactory;

void initXtermTerminalFactory(XtermTerminalFactory *terminalFactory, XTerm *rootTerm);

#endif
