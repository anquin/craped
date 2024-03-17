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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_CMD_STACK_MAX 2
#include "../../src/libsys/mem.c"
#include "../../src/libsys/raw_data.c"
#include "../../src/libsys/paging.c"
#include "../../src/libsys/page_factory.c"
#include "../../src/libsys/paged_raw_data.c"
#include "../../src/libsys/fileio.c"
#include "../../src/libsys/llfile.c"
#include "../../src/libsys/filesystem.c"
#include "../../src/libsys/stream.c"
#include "../../src/libsys/hashing.c"
#include "../../src/libsys/strutil.c"
#include "../../src/libsys/strhash.c"
#include "../../src/subeditor/buffer.c"
#include "../../src/subeditor/buffer_commands.c"
#include "../../src/subeditor/mark.c"

int main(void)
{
  static BufferCommand BUFFER_CMDS[] = {
    { BUF_CMD_INSERT, 0, 1, "a" },
    { BUF_CMD_INSERT, 1, 1, "b" },
    { BUF_CMD_INSERT, 2, 1, "c" },
    { BUF_CMD_INSERT, 3, 1, "d" }
  };
  size_t i;
  const size_t n = sizeof(BUFFER_CMDS) / sizeof(BufferCommand);
  BufferCmdStack stk;
  initBufferCmdStack(&stk);

  printf("N=%lu\n", n);
  for (i = 0; i < n; i++) {
    printf("Pushing %lu...\n", i);
    bufferCmdStackPush(&stk, &BUFFER_CMDS[i]);
    printf("Pushed %lu: %c.\n", i, BUFFER_CMDS[i].content[0]);
  }

  printf("I=%lu\n", i);
  for (i = 0; bufferCmdStackEmpty(&stk); i++) {
    printf("Popping %lu...\n", i-1);
    const BufferCommand *popped = bufferCmdStackPop(&stk);
    printf("Popped %lu: %c.\n", i-1, popped->content[0]);
  }

  return 0;
}
