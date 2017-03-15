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

#include "gtk-ui.h"
#include <diffexui/diffex-ui-private.h>
#include "gtkui/vte.h"

#include <libsys/dbglog.h>

#include <assert.h>

void
_get_input (GtkWidget *vteterminal, GdkEventKey *event, gpointer user_data);

ViewWindow *_create_view_window(GtkUI *gtkUi,
                                unsigned posX, unsigned posY,
                                unsigned sizeX, unsigned sizeY)
{
  ViewWindow *vw;
  GtkWidget *box;
  vw = gtkUi->grid.windows[posX][posY];
  assert(vw == NULL);
  vw = (ViewWindow *)malloc(sizeof(ViewWindow));
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
  gtk_widget_show(box);
  vw->wg = box;
  gtk_grid_attach(GTK_GRID(gtkUi->grid.wg),
                  box,
                  posX,
                  posY,
                  sizeX,
                  sizeY);
  gtkUi->grid.windows[posX][posY] = vw;
  return vw;
}

void initGtkUI(GtkUI *gtkui, const char *progName)
{
  int i, j;
  GtkWidget *gtkWin;
  GtkWidget *gtkGrid;

  gtk_init(NULL, NULL);

  gtkWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(gtkWin), progName);
  gtkGrid = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(gtkGrid), FALSE);
  gtk_grid_set_row_spacing(GTK_GRID(gtkGrid), 1);
  gtk_container_add(GTK_CONTAINER(gtkWin), gtkGrid);

  g_signal_connect_swapped(G_OBJECT(gtkWin), "destroy",
                           G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show(gtkGrid);
  gtk_widget_show(gtkWin);

  gtkui->gtkWin = gtkWin;
  gtkui->grid.wg = gtkGrid;

  VIEW_WINDOW_FOR_EACH_BEGIN(gtkui)
    gtkui->grid.windows[wndI][wndJ] = NULL;
  VIEW_WINDOW_FOR_EACH_END;
}

void destroyGtkUI(GtkUI *actualUi)
{
  gtk_main_quit();

  VIEW_WINDOW_FOR_EACH_BEGIN(actualUi)
    if (actualUi->grid.windows[wndI][wndJ] != NULL) {
      gtk_widget_destroy(actualUi->grid.windows[wndI][wndJ]->statusLineWg);
      gtk_widget_destroy(actualUi->grid.windows[wndI][wndJ]->terminalWg);
      gtk_widget_destroy(actualUi->grid.windows[wndI][wndJ]->wg);
      free(actualUi->grid.windows[wndI][wndJ]);
    }
  VIEW_WINDOW_FOR_EACH_END
}

void ActualUIPrepareTerminal(ActualUI *actualUi, DiffexUI *ui,
                             unsigned posX, unsigned posY,
                             unsigned sizeX, unsigned sizeY,
                             Terminal *terminal)
{
  fprintf(stderr, "ActualUIPrepareTerminal(%d, %d, %d, %d)\n",
          posX, posY, sizeX, sizeY);
  vteSetGridPosition(terminal, posX, posY);
  vteSetGridSize(terminal, sizeX, sizeY);
}

void ActualUIDrawTerminal(ActualUI *actualUi, DiffexUI *ui, Terminal *terminal)
{
  unsigned posX, posY;
  unsigned sizeX, sizeY;
  ViewWindow *vw;

  if (terminal == uiGetMiniTerminal_(ui)) {
    DBGLOG("AH!\n", 0);
    return;
  }

  vteGetGridPosition(terminal, &posX, &posY);
  vteGetGridSize(terminal, &sizeX, &sizeY);
  vw = actualUi->grid.windows[posX][posY];
  if (vw == NULL) {
    /* TODO: Can it be non-null? */
    vw = _create_view_window(actualUi, posX, posY, sizeX, sizeY);
  }

  gtk_box_pack_start(GTK_BOX(vw->wg), terminal->wg, TRUE, TRUE, 0);
  gtk_widget_show(terminal->wg);
}

void ActualUIEraseTerminal(ActualUI *actualUi, DiffexUI *ui, Terminal *terminal)
{
  unsigned posX, posY;
  ViewWindow *vw;

  if (terminal == uiGetMiniTerminal_(ui)) {
    return;
  }

  vteGetGridPosition(terminal, &posX, &posY);
  vw = actualUi->grid.windows[posX][posY];

  gtk_container_remove(GTK_CONTAINER(vw), terminal->wg);
}


void ActualUIDrawBox(ActualUI *actualUi,
                     DiffexUI *diffexUi,
                     Terminal *terminal)
{
  unsigned posX, posY;
  unsigned sizeX, sizeY;
  ViewWindow *vw;

  if (terminal == uiGetMiniTerminal_(diffexUi)) {
    return;
  }

  vteGetGridPosition(terminal, &posX, &posY);
  vteGetGridSize(terminal, &sizeX, &sizeY);
  vw = actualUi->grid.windows[posX][posY];
  vw->terminalWg = terminal->wg;
}

void ActualUIEraseBox(ActualUI *actualUi,
                      DiffexUI *diffexUi,
                      Terminal *terminal)
{
  unsigned oldPosX, oldPosY;
  ViewWindow *vw;
  GtkWidget *box;

  if (terminal == uiGetMiniTerminal_(diffexUi)) {
    return;
  }

  vteGetGridPosition(terminal, &oldPosX, &oldPosY);
  vw = actualUi->grid.windows[oldPosX][oldPosY];

  assert(vw != NULL);
  box = vw->wg;
  assert(box == gtk_widget_get_parent(terminal->wg));
  gtk_container_remove(GTK_CONTAINER(actualUi->grid.wg), box);
  gtk_widget_destroy(box);
  actualUi->grid.windows[oldPosX][oldPosY] = NULL;
}

void ActualUIDrawCenteredText(ActualUI *actualUi, DiffexUI *diffexUi,
                              char *text, unsigned sz)
{
  // TODO
}

void ActualUIEraseCenteredText(ActualUI *actualUi, DiffexUI *diffexUi,
                               char *text, unsigned sz)
{
  // TODO
}

unsigned ActualUIGetCenteredTextMax(ActualUI *actualUi, DiffexUI *diffexUi)
{
  // TODO
  return 256;
}

void ActualUIDrawStatusLine(ActualUI *actualUi, DiffexUI *ui,
                            unsigned wndPosX, unsigned wndPosY,
                            unsigned wndSizeX, unsigned wndSizeY,
                            char *text, unsigned size)
{
  GtkWidget *wndBox;
  GtkWidget *label;
  char *zs;

  printf("ActualUIDrawStatusLine(%d, %d, %d, %d)\n",
         wndPosX, wndPosY, wndSizeX, wndSizeY);

  /* FIXME: sad workaround for now, labels font size is different from VTE's.
   This causes the status bar to get increased therefore VTE is also resized. */
  size = 80;

  zs = (char *)malloc(sizeof(char) * size + 1);
  strncpy(zs, text, size);
  zs[size] = '\0';

  label = gtk_label_new(zs);

  /* wndBox = gtk_grid_get_child_at(GTK_GRID(actualUi->gtkGrid), wndPosX, wndPosY); */
  wndBox = actualUi->grid.windows[wndPosX][wndPosY]->wg;
  gtk_box_pack_end(GTK_BOX(wndBox), label, TRUE, TRUE, 0);
  gtk_box_set_child_packing(GTK_BOX(wndBox), label, 0, 0, 0, GTK_PACK_END);

  gtk_widget_show(label);
}

void ActualUIEraseStatusLine(ActualUI *actualUi, DiffexUI *ui,
                             unsigned wndPosX, unsigned wndPosY,
                             unsigned wndSizeX, unsigned wndSizeY,
                             char *text, unsigned size)
{
  GtkWidget *wndBox;
  GtkWidget *label;
  GList *boxChildren;
  GList *last;
  /* wndBox = gtk_grid_get_child_at(GTK_GRID(actualUi->gtkGrid), wndPosX, wndPosY); */
  wndBox = actualUi->grid.windows[wndPosX][wndPosY]->wg;
  boxChildren = gtk_container_get_children(GTK_CONTAINER(wndBox));
  last = g_list_last(boxChildren);
  if (GTK_IS_WIDGET(last->data)) {
    gtk_container_remove(GTK_CONTAINER(wndBox), GTK_WIDGET(last->data));
  }
}

void ActualUIMainLoop(ActualUI *actualUi, UI *ui)
{
  g_signal_connect(G_OBJECT(actualUi->gtkWin), "key_press_event",
                   G_CALLBACK(_get_input), ui);
  gtk_main();
}

void
_get_input (GtkWidget *vteterminal, GdkEventKey *event, gpointer user_data)
{
  KbInput *kbInput;
  int handled;
 
  handled = 0;
  if (event->type == GDK_KEY_PRESS) {
    kbInput = (KbInput *)malloc(sizeof(KbInput) * 2);
    kbInput->info = (char *)malloc(sizeof(char) * MULTIBYTE_CHAR_MAX_SIZE);
    kbInput->size = event->length;

    switch (event->state) {
    case GDK_CONTROL_MASK:
      kbInput->key = KEY_CTRL_SYMBOL;
      *((guint32 *)kbInput->info) = gdk_keyval_to_unicode(event->keyval);
      handled = 1;
      break;
    case GDK_MOD1_MASK:
    case GDK_META_MASK:
      kbInput->key = KEY_META_SYMBOL;
      *((guint32 *)kbInput->info) = gdk_keyval_to_unicode(event->keyval);
      handled = 1;
      break;
    }

    if (!handled) {
      switch (event->keyval) {
      case GDK_KEY_Return:
        kbInput->key = KEY_RETURN;
        kbInput->size = 0;
        free(kbInput->info);
        kbInput->info = NULL;
        handled = 1;
        break;
      case GDK_KEY_Control_L:
      case GDK_KEY_Control_R:
      case GDK_KEY_Meta_L:
      case GDK_KEY_Meta_R:
      case GDK_KEY_Alt_L:
      case GDK_KEY_Alt_R:
        break;
      case GDK_KEY_Left:
        kbInput->key = KEY_LEFT;
        kbInput->size = 0;
        kbInput->info = NULL;
        handled = 1;
        break;
      case GDK_KEY_Right:
        kbInput->key = KEY_RIGHT;
        kbInput->size = 0;
        kbInput->info = NULL;
        handled = 1;
        break;
      case GDK_KEY_Up:
        kbInput->key = KEY_UP;
        kbInput->size = 0;
        kbInput->info = NULL;
        handled = 1;
        break;
      case GDK_KEY_Down:
        kbInput->key = KEY_DOWN;
        kbInput->size = 0;
        kbInput->info = NULL;
        handled = 1;
        break;
      case GDK_KEY_BackSpace:
        kbInput->key = KEY_BACKSPACE;
        kbInput->size = 0;
        kbInput->info = NULL;
        handled = 1;
        break;
      case GDK_KEY_Delete:
        kbInput->key = KEY_DELETE;
        kbInput->size = 0;
        kbInput->info = NULL;
        handled = 1;
        break;
      default:
        kbInput->key = KEY_SYMBOL;
        *((guint32 *)kbInput->info) = gdk_keyval_to_unicode(event->keyval);
        handled = 1;
      }
    }

    if (handled) {
      kbInput[1].key = NO_KEY;
      uiObserverHandleInput(((DiffexUI *)user_data)->observer, kbInput);
    }

    /* DBG INFO*/
    /* if (kbInput->key == KEY_CTRL_SYMBOL) printf("C-"); */
    /* else if (kbInput->key == KEY_META_SYMBOL) printf("M-"); */
    /* fwrite(kbInput->info, 1, kbInput->size, stdout); */
    /* printf("[%d] ", kbInput->size); */
    /* int i; */
    /* for (i = 0; i < kbInput->size; i++) { */
    /*   printf("%x", kbInput->info[i]); */
    /* } */
    /* printf(" (%x ", event->keyval, event->string); */
    /* for (i = 0; i < event->length; i++) { */
    /*   printf("%x", event->string[i]); */
    /* } */
    /* printf(" %x", event->hardware_keycode); */
    /* printf(" %x", event->state); */
    /* printf(") \n"); */
    /* fflush(stdout); */
  }
}
