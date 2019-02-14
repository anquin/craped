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

#ifndef KEYS_H
#define KEYS_H

#include <libsys/hashing.h>

typedef enum keys
{
  NO_KEY=0,

  /* Positive Integers so a list can be zero-terminated */
  KEY_INSERT,
  KEY_DELETE,
  KEY_RETURN,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_SYMBOL,

  KEY_CTRL_INSERT,
  KEY_META_INSERT,
  KEY_CTRL_DELETE,
  KEY_META_DELETE,
  KEY_CTRL_RETURN,
  KEY_META_RETURN,
  KEY_CTRL_BACKSPACE,
  KEY_META_BACKSPACE,
  KEY_CTRL_LEFT,
  KEY_META_LEFT,
  KEY_CTRL_RIGHT,
  KEY_META_RIGHT,
  KEY_CTRL_UP,
  KEY_META_UP,
  KEY_CTRL_DOWN,
  KEY_META_DOWN,
  KEY_CTRL_SYMBOL,
  KEY_META_SYMBOL,

  KEY_MAX_VAL,

  KEY_UNKNOWN
} Key;

typedef struct kb_input
{
  Key key;
  char *info;
  unsigned size;
} KbInput;


KbInput *kbInputParse(char **keyStr);
Hashed *hash_kb_input(Hashed *hashed, KbInput *kb_input);

#endif
