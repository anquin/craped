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

/* TODO: isInput() for handling some multibyte. */

#include <stdlib.h>

#include "uicore/input.h"

/* #define KB_BACKSPACE    0x08 */
#define KB_BACKSPACE    0x7F
#define KB_TAB          0x09
#define KB_RETURN       0x0A
#define KB_META_PREFIX  0x1B


void translateRegularKey(char *info, int sz, KbInput *kbInput)
{
  unsigned ch, i;

  ch = 0;
  for (i = 0; i < 4; i++) {
    ch = ch | ((unsigned)info[i]) << (i << 3);
  }

  if (isprint(ch)) {
    kbInput->key = KEY_SYMBOL;
    kbInput->info = info;
    kbInput->size = sz;
  }
  else if (ch == KB_RETURN) {
    kbInput->key = KEY_RETURN;
    kbInput->info = NULL;
    kbInput->size = 0;
  }
  else if (ch == KB_TAB) {
    kbInput->key = KEY_TAB;
    kbInput->info = NULL;
    kbInput->size = 0;
  }
  else if (ch == KB_BACKSPACE) {
    kbInput->key = KEY_BACKSPACE;
    kbInput->info = NULL;
    kbInput->size = 0;
  }
  else if (ch <= 0x1a) {
    info[0] += 'a' - 1;
    kbInput->key = KEY_CTRL_SYMBOL;
    kbInput->info = info;
    kbInput->size = 1;
  }
  else {
    kbInput->key = KEY_SYMBOL;
    kbInput->info = info;
    kbInput->size = sz;
  }
}

void translateMetaKey(char *info, int sz, KbInput *kbInput)
{
  kbInput->key = KEY_META_SYMBOL;
  kbInput->info = info;
  kbInput->size = sz;
}

void translateFnKey(char *info, int sz, KbInput *kbInput)
{
  KbInput *moreInfo;

  switch (*((unsigned *)info)) {
  case 'D':
    kbInput->key = KEY_LEFT;
    kbInput->info = NULL;
    kbInput->size = 0;
    break;
  case 'C':
    kbInput->key = KEY_RIGHT;
    kbInput->info = NULL;
    kbInput->size = 0;
    break;
  case 'A':
    kbInput->key = KEY_UP;
    kbInput->info = NULL;
    kbInput->size = 0;
    break;
  case 'B':
    kbInput->key = KEY_DOWN;
    kbInput->info = NULL;
    kbInput->size = 0;
    break;
  case '3':
    /* if (isInput()) { */
    moreInfo = getInput();
    if (*(moreInfo->info) == 0x7e) {
      kbInput->key = KEY_DELETE;
      kbInput->info = NULL;
      kbInput->size = 0;
    }
  }
}

KbInput *getInput()
{
  int *ch;
  int nbytes;
  int i;
  short mode;
  void (*translate[3])(char *, int, KbInput *) = { translateRegularKey,
						   translateMetaKey,
						   translateFnKey };
  KbInput *kbInput;
  kbInput = (KbInput *)malloc(sizeof(KbInput) * 2);
  ch = (int *)malloc(sizeof(int));

  i = 0;
  *ch = 0;
  /* regular = 0, meta = 1 or function = 2 */
  mode = 0;
  mblen(NULL, 0);

  while (1) {
    *ch = (*ch) | getchar() << (i << 3);
    if (!i) {
      if (*ch == KB_META_PREFIX) {
        *ch = 0;
        mode = 1;
        continue;
      }
      else if (mode == 1 && *ch == 0x5b) {
        *ch = 0;
        mode = 2;
        continue;
      }
    }

    nbytes = mblen(((char *)ch), sizeof(int));

    if (nbytes > 0) {
      /* decode((char *)ch)
	 encode((char *)ch) */
      translate[mode]((char *)ch, nbytes, kbInput);
      break;

      i = 0;
      *ch = 0;
      mode = 0;
    }
    else if (nbytes == -1) {
      i += 1;
    }
    else {
      i = 0;
      *ch = 0;
      kbInput->key = KEY_UNKNOWN;
      kbInput->info = (char *)ch;
      kbInput->size = i;
      break;
    }
  }

  kbInput[1].key = NO_KEY;
  return kbInput;
}
