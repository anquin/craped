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

void bufCmdSetPoint(BufferCommand *bufCmd, Buffer *buf);
void bufCmdMovePoint(BufferCommand *bufCmd, Buffer *buf);
void bufCmdInsert(BufferCommand *bufCmd, Buffer *buf);
void bufCmdDelete(BufferCommand *bufCmd, Buffer *buf);

static struct buffer_cmd_type _BUF_CMD_SET_POINT;
static struct buffer_cmd_type _BUF_CMD_MOVE_POINT;
static struct buffer_cmd_type _BUF_CMD_INSERT;
static struct buffer_cmd_type _BUF_CMD_DELETE;

static struct buffer_cmd_type _BUF_CMD_SET_POINT =
  {
    "set_point",
    bufCmdSetPoint,
    &(BufferCommandCounts.bufCmdCountPointSet),
    &_BUF_CMD_SET_POINT
  };
BufferCmdType *BUF_CMD_SET_POINT = &_BUF_CMD_SET_POINT;

static struct buffer_cmd_type _BUF_CMD_MOVE_POINT =
  {
    "point_move",
    bufCmdMovePoint,
    &(BufferCommandCounts.bufCmdCountPointMove),
    &_BUF_CMD_MOVE_POINT
  };
BufferCmdType *BUF_CMD_MOVE_POINT = &_BUF_CMD_MOVE_POINT;

static struct buffer_cmd_type _BUF_CMD_INSERT =
  {
    "insert_string",
    bufCmdInsert,
    &(BufferCommandCounts.bufCmdCountInsert),
    &_BUF_CMD_DELETE
  };
BufferCmdType *BUF_CMD_INSERT = &_BUF_CMD_INSERT;

static struct buffer_cmd_type _BUF_CMD_DELETE =
  {
    "delete",
    bufCmdDelete,
    &(BufferCommandCounts.bufCmdCountDelete),
    &(_BUF_CMD_INSERT)
  };
BufferCmdType *BUF_CMD_DELETE = &_BUF_CMD_DELETE;

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

void bufCmdSetPoint(BufferCommand *bufCmd, Buffer *buf)
{
  bufferSetPoint(buf, bufCmd->bytePos);
}

void bufCmdMovePoint(BufferCommand *bufCmd, Buffer *buf)
{
  bufferMovePoint(buf, bufCmd->opSize);
}

void bufCmdInsert(BufferCommand *bufCmd, Buffer *buf)
{
  bufferInsert(buf, bufCmd->content, bufCmd->opSize);
}

void bufCmdDelete(BufferCommand *bufCmd, Buffer *buf)
{
  bufferDelete(buf, bufCmd->opSize);
}

void initBufferCmdStack(BufferCmdStack *bufCmdStk)
{
  bufCmdStk->max = BUFFER_CMD_STACK_MAX;
  bufCmdStk->top = BUFFER_CMD_STACK_MAX;
}

void bufferCmdStackPush(BufferCmdStack *stk, BufferCommand *cmd)
{
  if (stk->top < stk->max) {
    stk->stack[--stk->top] = cmd;
  }
}

BufferCommand *bufferCmdStackPop(BufferCmdStack *stk)
{
  if (stk->top > 0) {
    return stk->stack[stk->top--];
  }
  return NULL;
}
