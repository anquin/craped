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

#ifndef BUFFER_COMMANDS_H
#define BUFFER_COMMANDS_H

#define CMD_NAME_MAX 128

typedef unsigned long long CommandCount;

struct buffer_command;

typedef struct buffer_cmd_type
{
  char name[CMD_NAME_MAX];
  void (*execute)(struct buffer_command *, Buffer *);
  CommandCount *timesUsed;

  /* CommandType to rollback */
  struct buffer_cmd_type *rowbackCmd;

} BufferCmdType;

extern BufferCmdType *BUF_CMD_SET_POINT;
extern BufferCmdType *BUF_CMD_MOVE_POINT_FORWARD;
extern BufferCmdType *BUF_CMD_MOVE_POINT_BACKWARD;
extern BufferCmdType *BUF_CMD_INSERT;
extern BufferCmdType *BUF_CMD_DELETE;

typedef struct buffer_command
{
  BufferCmdType *type;

  /* == Buffer State Infos == */
  Position bytePos;
  /* Size of the operation, eg.: how many bytes do ins/del */
  Size opSize;
  Byte *content;

} BufferCommand;

#define bufferCommandExec(bufCmd, buf) (*bufCmd).type->execute(bufCmd, buf)

BufferCommand *createBufferCommand(BufferCmdType *type, Position bytePos,
				   Size opSize, Byte *content);

void initBufferCommand(BufferCommand *bufCmd, BufferCmdType *type,
		       Position bytePos, Size opSize, Byte *content);
void destroyBufferCommand(BufferCommand *bufCmd);

struct
{
  CommandCount bufCmdCountPointSet;
  CommandCount bufCmdCountPointMoveForward;
  CommandCount bufCmdCountPointMoveBackward;
  CommandCount bufCmdCountInsert;
  CommandCount bufCmdCountDelete;
} BufferCommandCounts;

#define BUFFER_CMD_STACK_MAX 256

typedef struct buffer_cmd_stack
{
  unsigned max;
  unsigned top;
  BufferCommand *stack[BUFFER_CMD_STACK_MAX];
} BufferCmdStack;

void initBufferCmdStack(BufferCmdStack *bufCmdStk);
void bufferCmdStackPush(BufferCmdStack *stk, BufferCommand *cmd);
BufferCommand *bufferCmdStackPop(BufferCmdStack *stk);

#endif
