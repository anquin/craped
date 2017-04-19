#/*
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
#include <string.h>
#include <assert.h>

#include <subeditor.h>
#include <uicore/buffer_point_moves.h>
#include <subeditor/mode.h>

unsigned terminalGetHeight(struct terminal *t) {}
void observerUpdate(struct world_observer *wo, World *w) {}
void
fetchBytes(World *world,
           MoveBufferPointFn moveFn,
           char *line,
           unsigned *nbytes);

int main(void)
{
  int i;
  World *world = createWorld(NULL);
  const char *lines[] = {"1 Line 1\n", "2 Line 2\n", "\n", "3 Line 3\n"};
  size_t n_lines = sizeof(lines) / sizeof(char *);

  for (i = 0; i < n_lines; i++) {
    worldInsert(world, (Byte *)lines[i], strlen(lines[i]));
  }

  worldSetPoint(world, 0);

  char line[128]; unsigned nbytes;
  for (i = 0; i < n_lines; i++) {
    fetchBytes(world, utf8LineMovePoint, line, &nbytes);
    assert(!strncmp(line, lines[i], nbytes));
  }

  destroyWorld(world);
  return 0;
}

void
fetchBytes(World *world,
           MoveBufferPointFn moveFn,
           char *line,
           unsigned *nbytes)
{
  Position beg, end;
  Size sz;
  *nbytes = 0;
  beg = worldGetPoint(world);
  if (beg >= worldBufferSize(world)) {
    return;
  }
  moveFn(world, 1);
  end = worldGetPoint(world);
  worldSetPoint(world, beg);
  sz = end - beg;
  worldGetChunk(world, (Byte *)line, sz);
  worldSetPoint(world, end);
  *nbytes = sz;
  if (line[sz - 1] == '\n') {
    *nbytes++;
  }
}
