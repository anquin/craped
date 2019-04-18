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

#ifndef UI_PRIVATE_H
#define UI_PRIVATE_H

#include <uicore/ui.h>
#include <uicore/terminal.h>
#include <uicore/windowmanager.h>
#include <uicore/window.h>

/* Returns window's terminal or NULL if can't find it. */
Terminal *uiGetWindowTerminalById_(UI *ui, unsigned winId);

WindowManager *uiGetWindowManager_(UI *ui);

void uiGetCenteredMessage_(UI *ui, char **message, unsigned *size);

void uiBuildStatusLine_(UI *ui, Window *window, World *world,
                        char **line, unsigned *size);

/* TODO: remove */
/* Terminal *uiGetMiniTerminal_(UI *ui); */

#endif
