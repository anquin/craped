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

#include "subeditor/def.h"
#include "buffer.h"
#include "buffer_commands.h"

#include <stdlib.h>
#include <string.h>

struct buffer_command_counts BufferCommandCounts;
int bufCmdInsert(BufferCommand *bufCmd, Buffer *buf);
int bufCmdDelete(BufferCommand *bufCmd, Buffer *buf);

static struct buffer_cmd_type _BUF_CMD_INSERT;
static struct buffer_cmd_type _BUF_CMD_DELETE;

static struct buffer_cmd_type _BUF_CMD_INSERT =
  {
    "insert_string",
    bufCmdInsert,
    &(BufferCommandCounts.bufCmdCountInsert),
    &_BUF_CMD_DELETE
  };
const BufferCmdType * const BUF_CMD_INSERT = &_BUF_CMD_INSERT;

static struct buffer_cmd_type _BUF_CMD_DELETE =
  {
    "delete",
    bufCmdDelete,
    &(BufferCommandCounts.bufCmdCountDelete),
    &(_BUF_CMD_INSERT)
  };
const BufferCmdType * const BUF_CMD_DELETE = &_BUF_CMD_DELETE;

BufferCommand *createBufferCommand(BufferCmdType *type, Position bytePos,
		    Size opSize, Byte *content)
{
  BufferCommand *bufCmd;

  bufCmd = (BufferCommand *)malloc(sizeof(BufferCommand));
  initBufferCommand(bufCmd, type, bytePos, opSize, content);

  return bufCmd;
}

void initBufferCommand(BufferCommand *bufCmd, BufferCmdType *type,
		       Position bytePos, Size opSize, Byte *content)
{
  bufCmd->type = type;
  bufCmd->bytePos = bytePos;
  bufCmd->opSize = opSize;
  bufCmd->content = content;
}

void destroyBufferCommand(BufferCommand *bufCmd)
{
  /* No need to do something */
}

int bufCmdInsert(BufferCommand *bufCmd, Buffer *buf)
{
  if (!bufCmd->opSize) return 0;
  if (buf->flags & BUFFER_FLAG_RDONLY) return -1;

  bufferSetPoint(buf, bufCmd->bytePos);
  return (bufferInsert(buf, bufCmd->content, bufCmd->opSize)
          == bufCmd->opSize) ? 1 : -1;
}

int bufCmdDelete(BufferCommand *bufCmd, Buffer *buf)
{
  if (!bufCmd->opSize) return 0;
  if (buf->flags & BUFFER_FLAG_RDONLY) return -1;

  bufCmd->content = (Byte *)malloc(sizeof(bufCmd->opSize));
  bufferSetPoint(buf, bufCmd->bytePos);
  bufferGetChunk(buf, bufCmd->content, bufCmd->opSize);
  return (bufferDelete(buf, bufCmd->opSize) == bufCmd->opSize)
    ? 1 : -1;
}

void initBufferCmdStack(BufferCmdStack *bufCmdStk)
{
  bufCmdStk->max = BUFFER_CMD_STACK_MAX;
  bufCmdStk->areasz = BUFFER_CMD_STACK_AREA_SZ;
  bufCmdStk->top = bufCmdStk->max;
  bufCmdStk->stack = bufCmdStk->area + bufCmdStk->max;
}

void bufferCmdStackPush(BufferCmdStack *stk, BufferCommand *cmd)
{
  if (stk->stack == stk->area) {
    stk->stack = stk->area + stk->max;
    memcpy(stk->stack, stk->area, stk->max);
    stk->top = stk->max;
  }
  stk->stack[--stk->top] = cmd;
  if (stk->top < 0) {
    stk->stack += stk->top;
    stk->top = 0;
  }
}

BufferCommand *bufferCmdStackPop(BufferCmdStack *stk)
{
  if (stk->top < stk->max) {
    return stk->stack[stk->top++];
  }
  return NULL;
}

int bufferCmdStackEmpty(BufferCmdStack *stk)
{
  return (stk->top >= stk->max);
}
