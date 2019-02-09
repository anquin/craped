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

#include "testcommons.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "ui/ui.h"
#include "editor/editor_impl.h"

/* Creates a fake UI */
typedef struct world_observer {
} FakeUI;
void uiRedisplay(UI *ui, World *w){}
struct window *uiGetWindow(UI *ui){}
void uiSayHello(UI *ui, char *s){}
void uiMainLoop(UI *ui){}
void uiSetObserver(UI *ui, struct editor *editor){}
void uiNextWindow(UI *ui){}
void uiPrevWindow(UI *ui){}
void uiRemoveCurrentWindow(UI *ui){}
void uiSplitWindowHorz(UI *ui){}
void uiSetWindowHR(UI *ui, short h){}
void uiSetWindowBuffer(UI *ui, char *bufName){}
char *uiGetWindowBufferName(UI *ui){return "*scratch*";}
unsigned uiGetWindowHeight(UI *ui){}
void observerUpdate(FakeUI *textUi, World *world){}
void uiSayCentered(UI *ui, char *txt){}
void uiActivateMiniWindow(UI *ui, short flag){}


int main(void)
{
  FakeUI ui;
  Editor *editor;
  FILE *fin;
  char c;
  char *tmpfilepath;
  tmpfilepath = tmpnam(NULL);

  KbInput kbInput[3];
  kbInput[0].key = KEY_CTRL_SYMBOL;
  kbInput[0].info = "y";
  kbInput[0].size = 1;
  kbInput[1].key = NO_KEY;

  editor = (Editor *)malloc(sizeof(Editor));
  initEditor(editor, &ui);
  editorInsert(editor, "z", 1);
  editorSetBufferFilePath(editor, tmpfilepath);
  editorBindKeyCombo(editor, "C-y", "write_buffer");
  editorKeyHandle(editor, kbInput);
  fin = fopen(tmpfilepath, "rb");
  assert(fin != NULL);
  fread(&c, sizeof(char), 1, fin);
  assert(c == 'z');

  printf("Key binding test passed.\n");
  return 0;
}
