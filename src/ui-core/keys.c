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

#include "uicore/keys.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <libsys/strhash.h>
#include <libsys/mem.h>

int kbInputEqualsFn(KbInput *this, KbInput *other)
{
  if (this->key != other->key) {
    return 1;
  }
  if (this->size != other->size) {
    return 1;
  }
  return memcmp(this->info, other->info, this->size);
}

Hash hash_kb_input_fn(KbInput *kbInput)
{
  Hash hash;
  char *info = (char *)kbInput->info;
  unsigned i;

  hash = kbInput->key;

  for (i = 0; i < kbInput->size; i++) {
    hash += (unsigned)(31 * info[i]);
  }

  return hash;
}

Hashed *hash_kb_input(Hashed *hashed, KbInput *kb_input)
{
  Hash hash = hash_kb_input_fn(kb_input);
  return hashed_init(hashed, hash, kb_input,
                     (int (*)(void *, void *))kbInputEqualsFn);
}

Key keyGetKeyByName(char *name)
{
  if (!strcmp(name, "insert")) {
    return KEY_INSERT;
  }
  if (!strcmp(name, "delete")) {
    return KEY_DELETE;
  }
  if (!strcmp(name, "return")) {
    return KEY_RETURN;
  }
  if (!strcmp(name, "backspace")) {
    return KEY_BACKSPACE;
  }
  if (!strcmp(name, "tab")) {
    return KEY_TAB;
  }
  if (!strcmp(name, "leftarrow")) {
    return KEY_LEFT;
  }
  if (!strcmp(name, "rightarrow")) {
    return KEY_RIGHT;
  }
  if (!strcmp(name, "uparrow")) {
    return KEY_UP;
  }
  if (!strcmp(name, "downarrow")) {
    return KEY_DOWN;
  }

  return KEY_SYMBOL;
}

KbInput *kbInputParse(char **keyStr)
{
  KbInput *kbInput, *aux;
  kbInput = (KbInput *)malloc(sizeof(KbInput));

  if (*(*keyStr + 1) == '-') {
    switch (**keyStr) {
    case 'C':
      kbInput->key = KEY_CTRL_SYMBOL;
      break;
    case 'M':
      kbInput->key = KEY_META_SYMBOL;
      break;
    default:
      /* TODO */
      abort();
    }
    *keyStr += 2;
    aux = kbInputParse(keyStr);

    if (aux->key == KEY_SYMBOL) {
      kbInput->info = (char *)malloc(sizeof(char) * aux->size);
      memcpy(kbInput->info, aux->info, aux->size);
      kbInput->size = aux->size;
    }
  }
  else {
    char *keyName;
    int i;
    i = 0;
    while ((*keyStr)[i] != ' ' && ((*keyStr)[i] != '\0')) {
      i++;
    }
    keyName = (char *)malloc(sizeof(char) * i);
    strncpy(keyName, *keyStr, i);
    kbInput->key = keyGetKeyByName(keyName);
    assert(kbInput->key != KEY_UNKNOWN);
    if (kbInput->key == KEY_SYMBOL) {
      kbInput->info = keyName;
      kbInput->size = i;
    }
    *keyStr += i;
  }

  return kbInput;
}
