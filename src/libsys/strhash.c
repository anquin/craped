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

#include <string.h>
#include <libsys/strhash.h>
#include <libsys/mem.h>
#include <libsys/strutil.h>

/* Of course this can be improved */
Hash hash_string_fn(char *str)
{
  Hash hash;

  hash = 0;
  while (*(str++)) {
    hash += (Hash)(31 * (*str));
  }

  return hash;
}

Hashed *hash_string(Hashed *hashed, char *str)
{
  Hash hash = hash_string_fn(str);
  return hashed_init(hashed, hash, copy_str(str),
                     (int (*)(void *, void *))strcmp);
}
