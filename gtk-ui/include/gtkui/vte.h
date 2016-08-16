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

#ifndef VTE_H
#define VTE_H

#include <uicore/terminal.h>

#include <gtk/gtk.h>

typedef struct terminal
{
  GtkWidget *wg;

  unsigned gridPosX, gridPosY;
  unsigned gridSizeX, gridSizeY;
} VTE;

Terminal *createVte();
void initVte(VTE *term);

void vteSetSize(VTE *term, int cols, int rows);

#define vteSetGridPosition(term, pX, pY) do {                           \
    term->gridPosX=pX;                                                  \
    term->gridPosY=pY;                                                  \
  }while(0)
#define vteGetGridPosition(term, pX, pY) do {                           \
    *((unsigned *)pX) = term->gridPosX;                                 \
    *((unsigned *)pY) = term->gridPosY;                                 \
  }while(0)
#define vteSetGridSize(term, sX, sY) do {       \
    term->gridSizeX=sX;                         \
    term->gridSizeY=sY;                         \
  }while(0)
#define vteGetGridSize(term, sX, sY) do {       \
  *((unsigned *)sX) = term->gridSizeX;          \
  *((unsigned *)sY) = term->gridSizeY;          \
  }while(0)

#endif
