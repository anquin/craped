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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "subeditor/internal/world.h"

#define BUFFERS_MAX 10

extern Size maxPageSize;

void observerUpdate(struct world_observer *editor, World *world)
{
  /* Just needs implementation, does nothing. */
}

void printWorld(World *world)
{
  char *str;
  str = (char *)malloc(sizeof(char) * worldBufferSize(world));
  worldAddMark(world, "m1");
  worldSetPoint(world, 0);
  worldGetChunk(world, (Byte *)str, worldBufferSize(world));
  worldMarkToPoint(world, "m1");
  worldRemoveMark(world, "m1");
  printf("%s\n", str);
}

unsigned randStr(char **str, int min, int max)
{
  unsigned sz;
  unsigned i;
  static char
    lexic[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
  static unsigned lexSz = 0;

  if (!lexSz) {
    lexSz = strlen(lexic);
  }

  sz = rand() % (max - min) + min;
  *str = malloc(sizeof(char) * sz);

  for (i = 0; i < sz; i++) {
    (*str)[i] = lexic[rand() % lexSz];
  }
}

#define SAVE_POINT(world) worldAddMark(world, "m1");worldSetPoint(world, 0)
#define LOAD_POINT(world) worldMarkToPoint(world, "m1");worldRemoveMark(world, "m1")

int main(void)
{
  World world;
  char *bufNames[BUFFERS_MAX];
  int i;
  unsigned nbytes;
  char *content;

  /* Sets the page size to a low value to increase the risks */
  maxPageSize = 16;

  srand(time(0));

  initWorld(&world, NULL);

  /* printf("BUFFER: %s\n", worldGetBufferName(&world)); */

  content = (char *)malloc(sizeof(content));
  worldInsert(&world, "uouou!", 7);
  SAVE_POINT(&world);
  assert(worldGetChunk(&world, content, 7) == 7);
  LOAD_POINT(&world);
  assert(!strcmp(content, "uouou!"));
  /* Let's desconsider the null byte... */
  worldMovePoint(&world, -1);
  /* printWorld(&world); */

  worldMovePoint(&world, -6);
  worldDelete(&world, 1);
  assert(worldBufferSize(&world) == 6);
  assert(worldGetChunk(&world, content, 7) == 6);
  assert(!strcmp(content, "ouou!"));
  /* printWorld(&world); */

  worldInsert(&world, "U", 1);
  assert(worldBufferSize(&world) == 7);
  SAVE_POINT(&world);
  assert(worldGetChunk(&world, content, 7) == 7);
  LOAD_POINT(&world);
  assert(!strcmp(content, "Uouou!"));
  /* printWorld(&world); */

  worldMovePoint(&world, 1);
  worldDelete(&world, 1);
  assert(worldBufferSize(&world) == 6);
  SAVE_POINT(&world);
  assert(worldGetChunk(&world, content, 7) == 6);
  LOAD_POINT(&world);
  assert(!strcmp(content, "Uoou!"));
  /* printWorld(&world); */

  worldInsert(&world, "U", 1);
  assert(worldBufferSize(&world) == 7);
  SAVE_POINT(&world);
  assert(worldGetChunk(&world, content, 7) == 7);
  LOAD_POINT(&world);
  assert(!strcmp(content, "UoUou!"));
  /* printWorld(&world); */

  for (i = 0; i < BUFFERS_MAX; i++) {
    randStr(&bufNames[i], 1, 16);
    worldCreateBuffer(&world, bufNames[i]);
    worldSetCurrentBuffer(&world, bufNames[i], 0);
    worldAddMark(&world, "m1");
    nbytes = randStr(&content, 100, 32767);
    worldInsert(&world, content, nbytes);
    /* printf("%d) BUFFER: %s\n", i, worldGetBufferName(&world)); */
    assert(!strcmp(bufNames[i], worldGetBufferName(&world)));
    /* printWorld(&world); */
  }

  printf("\n");
  for (i = 0; i < BUFFERS_MAX; i++) {
    int bufIdx = rand() % BUFFERS_MAX;
    worldSetCurrentBuffer(&world, bufNames[bufIdx], 0);
    /* printf("rand%d) BUFFER: %s\n", i, worldGetBufferName(&world)); */
    assert(!strcmp(bufNames[bufIdx], worldGetBufferName(&world)));
  }

  destroyWorld(&world);
  return 0;
}
