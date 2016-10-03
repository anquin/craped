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

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

/* Window type must be known outside but its operations and ttributes must not. */
/* The main need here is to let the outside world reference the windows. */
/* Let the outside world reference the windows by id is not a good idea*/
/* because ids are always reused when a window gets destroyed. */

typedef struct window Window;

/* These are (hopefully) temporary functions needed */
/* while mode doesn't make its way to the buffer structure */
#include <subeditor/mode.h>
void windowSetMode(Window *, Mode *);
Mode *windowGetMode(Window *window);

#endif
