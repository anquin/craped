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

/*
  REFERENCE:
  http://www.valadoc.org/#!api=gtk+-3.0/Gtk.Grid
  https://developer.gnome.org/gtk3/stable/GtkGrid.html#gtk-grid-attach
  https://developer.gnome.org/gtk3/stable/GtkContainer.html#gtk-container-remove
*/

#include "gtkui/vte.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <vte/vte.h>


void terminalAllocMap_(Terminal *term);
void terminalFreeMap_(Terminal *term);

Terminal *createVte()
{
  VTE *term;
  term = (VTE *)malloc(sizeof(VTE));
  initVte(term);
  return term;
}

void
initVte(Terminal *term)
{
  GdkColor black = {0, 0x0000, 0x0000, 0x0000};
  GdkColor white = {0, 0xFFFF, 0xFFFF, 0xFFFF};

  term->wg = vte_terminal_new();
  vte_terminal_set_color_foreground(VTE_TERMINAL(term->wg), &white);
  vte_terminal_set_color_background(VTE_TERMINAL(term->wg), &black);
  vte_terminal_set_encoding(VTE_TERMINAL(term->wg), "UTF-8");
  vte_terminal_set_emulation(VTE_TERMINAL(term->wg), "xterm");

  gtk_widget_set_vexpand(term->wg, TRUE);
  gtk_widget_set_hexpand(term->wg, TRUE);

  terminalClear(term);
}

void destroyTerminal(Terminal *term)
{
  gtk_widget_destroy(term->wg);
  term->wg = NULL;
}

void terminalClear(Terminal *term)
{
  vte_terminal_feed(VTE_TERMINAL(term->wg), "\x1b[1;1H\x1b[2J", 10);
}

void terminalSetCursor(Terminal *term, unsigned x, unsigned y)
{
  char cmdLine[20] = {'\0'};
  if (((int)x) >= 0 && ((int)y) >= 0) {
    sprintf(cmdLine, "\x1b[%d;%dH", y + 1, x + 1);
    vte_terminal_feed(VTE_TERMINAL(term->wg), cmdLine, strlen(cmdLine));
  }
}

void terminalGetCursor(Terminal *term, unsigned *x, unsigned *y)
{
  glong col, row;
  vte_terminal_get_cursor_position(VTE_TERMINAL(term->wg), &col, &row);
  *x = col;
  *y = row;
}

void terminalClearEndOfLine(Terminal *term)
{
  unsigned row, col;
  unsigned width;
  static char vt100str[] = "\x0b";

  width = terminalGetWidth(term);
  terminalGetCursor(term, &col, &row);
  ++col;
  printf("CLEOL((%d, %d) %d)\n", col, row, width - col);

  terminalWrite(term, vt100str, sizeof(vt100str));
}

void terminalWrite(Terminal *term, Byte *info, Size size)
{
  vte_terminal_feed(VTE_TERMINAL(term->wg), info, size);
}

void terminalDelete(Terminal *term, Size size)
{
  char s[32];
  int len;

  sprintf(s, "\x1b[%dP", size);
  len = strlen(s);
  terminalWrite(term, s, len);
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
  return vte_terminal_get_column_count(VTE_TERMINAL(term->wg));
}

unsigned terminalGetHeight(Terminal *term)
{
  return vte_terminal_get_row_count(VTE_TERMINAL(term->wg));
}

void terminalFlush(Terminal *term)
{
}

void terminalGrabFocus(Terminal *terminal)
{
  gtk_widget_grab_focus(terminal->wg);
}

void terminalDrawBox(Terminal *term, struct box *box)
{
  /* not needed */
}

void terminalEraseBox(Terminal *term, struct box *box)
{
  /* not needed */
}

void vteSetSize(VTE *term, int cols, int rows)
{
  vte_terminal_set_size(VTE_TERMINAL(term->wg), cols, rows);
}
