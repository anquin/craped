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

#include <stdlib.h>
#include <stdio.h>

/* Needed because libcraped is not yet installed */
#include "../libsys/include/libsys/def.h"

#include <craped.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int main(int argc, char *argv[])
{
  const char progName[] = PACKAGE_STRING;
  Craped *craped;
  CrapedSubscriber *subscribers[] = { NULL };
  craped = createCraped(argc, argv, subscribers, progName);
  crapedRun(craped);
  destroyCraped(craped);
  free(craped);
  return 0;
}
