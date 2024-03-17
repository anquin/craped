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

#include "undo.h"

/* Returns 0 if ok */
static
int genRevertCmd_(Buffer *buf, BufferCommand *cmd, BufferCommand *revertCmd)
{
  if (cmd->type == BUF_CMD_INSERT) {
    initBufferCommand(revertCmd,
                      BUF_CMD_DELETE,
                      cmd->bytePos,
                      cmd->opSize,
                      cmd->content);
    return 0;
  } else if (cmd->type == BUF_CMD_DELETE) {
    initBufferCommand(revertCmd,
                      BUF_CMD_INSERT,
                      cmd->bytePos,
                      cmd->opSize,
                      cmd->content);
    return 0;
  }
  return 1;
}

int undo(Buffer *buf, BufferCommand *cmd)
{
  int cmdReturn;
  BufferCommand revertCmd;
  if (!genRevertCmd_(buf, cmd, &revertCmd)) {
    cmdReturn = bufferCommandExec(&revertCmd, buf);
    /* TODO: shouldn't destroy but be saved somewhere for enabling redoing */
    destroyBufferCommand(&revertCmd);
    return cmdReturn;
  }
  return -1;
}
