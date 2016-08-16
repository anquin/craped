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
#include <assert.h>

#include <buffer.h>
#include <buffer_commands.h>

#define CMDS_COUNT 4

void observerUpdate(struct world_observer *wo, struct world *w)
{
}

void fnprint(Byte *what, Size n, FILE *f)
{
  Size i;

  for (i = 0; i < n; i++) {
    fprintf(f, "%c", what[i]);
  }
  fprintf(f, " (%d bytes)\n", n);
}

typedef struct
{
  char *str;
  int size;
} ExpectedResult;

int main(void)
{
  Buffer buffer;
  BufferCommand bufCmds[CMDS_COUNT];
  Size bufSize;
  Byte *tmp;
  unsigned i;
  char *mark1 = "*tmp*";
  ExpectedResult expected[] = {{"aaAAsAAaa", 9},
                               {"aaAAsAAaa", 9},
                               {"aaAAAAaa", 8},
                               {"aaAA***AAaa", 11}};

  initBuffer(&buffer, "teste");

  initBufferCommand(&bufCmds[0], BUF_CMD_INSERT, 0, 9, "aaAAsAAaa");
  initBufferCommand(&bufCmds[1], BUF_CMD_MOVE_POINT, 0, -5, NULL);
  initBufferCommand(&bufCmds[2], BUF_CMD_DELETE, 0, 1, NULL);
  initBufferCommand(&bufCmds[3], BUF_CMD_INSERT, 0, 3, "***");

  bufSize = bufferSize(&buffer);
  tmp = (Byte *)malloc(sizeof(Byte)*9);
  assert(tmp != NULL);

  bufferAddMark(&buffer, mark1, MARK_FLAG_FIXED);

  for (i = 0; i < CMDS_COUNT; i++) {
    bufferCommandExec(&(bufCmds[i]), &buffer);
    bufSize = bufferSize(&buffer);
    if (bufSize) {
      tmp = (Byte *)realloc(tmp, sizeof(Byte)*bufSize);
    }

    bufferPointToMark(&buffer, mark1);
    bufferSetPoint(&buffer, 0);
    bufSize = bufferGetChunk(&buffer, tmp, bufSize);
    /* fnprint(tmp, bufSize, stdout); */
    bufferMarkToPoint(&buffer, mark1);

    assert(bufSize == expected[i].size);
    assert(!strncmp(tmp, expected[i].str, bufSize));
  }

  bufferRemoveMark(&buffer, mark1);

  return 0;
}
