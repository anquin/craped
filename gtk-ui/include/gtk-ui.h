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

#ifndef GTK_UI_H
#define GTK_UI_H

#include <diffex-ui.h>
#include <uicore/terminal.h>

#include <gtk/gtk.h>

typedef struct view_window
{
  GtkWidget *wg;
  GtkWidget *terminalWg;
  GtkWidget *statusLineWg;
} ViewWindow;

#define VIEW_WINDOW_FOR_EACH_BEGIN(gtkui) {           \
  int wndI, wndJ;                                     \
  for (wndI = 0;                                      \
       wndI < (sizeof(gtkui->grid.windows)            \
               / sizeof(gtkui->grid.windows[0]));     \
       wndI++) {                                      \
    for (wndJ = 0;                                    \
         wndJ < (sizeof(gtkui->grid.windows[0])       \
                 / sizeof(ViewWindow *));             \
         wndJ++) {
#define VIEW_WINDOW_FOR_EACH_END }}}

typedef struct view_grid
{
  ViewWindow *windows[10][10];
  GtkWidget *wg;
} ViewGrid;

typedef struct actual_ui
{
  ViewGrid grid;
  GtkWidget *gtkWin;
} GtkUI;

void initGtkUI(GtkUI *gtkUi, const char *progName);
void destroyGtkUI(GtkUI *gtkUi);

#endif
