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

  /* Returns:
     <0 if no command was ran
     0 if buf wasn't changed
     >0 if buffer was changed */
  int (*execute)(struct buffer_command *, Buffer *);

  /* For statistics(?) */
  CommandCount *timesUsed;

  /* CommandType to rollback */
  struct buffer_cmd_type *rowbackCmd;

} BufferCmdType;

extern const BufferCmdType * const BUF_CMD_INSERT;
extern const BufferCmdType * const BUF_CMD_DELETE;

typedef struct buffer_command
{
  const BufferCmdType *type;

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

extern struct buffer_command_counts
{
  CommandCount bufCmdCountInsert;
  CommandCount bufCmdCountDelete;
} BufferCommandCounts;

#ifndef BUFFER_CMD_STACK_MAX
#define BUFFER_CMD_STACK_MAX 256
#endif
#define BUFFER_CMD_STACK_AREA_SZ (BUFFER_CMD_STACK_MAX * 2)

typedef struct buffer_cmd_stack
{
  int top;
  unsigned max;
  unsigned areasz;
  BufferCommand *area[BUFFER_CMD_STACK_AREA_SZ];
  BufferCommand **stack;
} BufferCmdStack;

void initBufferCmdStack(BufferCmdStack *bufCmdStk);
void bufferCmdStackPush(BufferCmdStack *stk, BufferCommand *cmd);
BufferCommand *bufferCmdStackPop(BufferCmdStack *stk);
int bufferCmdStackEmpty(BufferCmdStack *stk);

#endif
