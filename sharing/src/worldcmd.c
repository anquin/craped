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

#include <sharing/worldcmd.h>

#include <stdlib.h>
#include <string.h>

WorldCmdExecuteFn worldCmds[] = { worldCmdDestroy,
				  worldCmdInsertExec,
				  worldCmdDeleteExec,
				  worldCmdMovePointForward,
				  worldCmdMovePointBackward,
				  worldCmdSetPoint };

WorldCmd *
createWorldCmd(WorldCmdType type,
	       Position position,
	       Size size,
	       Size contentSize,
	       Byte *content,
               char *bufferName)
{
  WorldCmd *worldCmd = (WorldCmd *)malloc(sizeof(WorldCmd));
  initWorldCmd(worldCmd, type, position, size,
               contentSize, content, bufferName);
  return worldCmd;
}

void
initWorldCmd(WorldCmd *cmd,
	     WorldCmdType type,
	     Position position,
	     Size size,
	     Size contentSize,
	     Byte *content,
             char *bufferName)
{
  cmd->type = type;
  cmd->position = position;
  cmd->size = size;
  cmd->contentSize = contentSize;
  cmd->content = (Byte *)malloc(sizeof(Byte) * contentSize);
  memcpy(cmd->content, content, contentSize);
  if (bufferName != NULL) {
    strncpy(cmd->bufferName, bufferName, WORLD_MAX_BUFFER_NAME);
  }
  else {
    *cmd->bufferName = '\0';
  }
}

void copyWorldCmd(WorldCmd *dest, WorldCmd *src)
{
  dest->type = src->type;
  dest->position = src->position;
  dest->size = src->size;
  dest->contentSize = src->contentSize;
  dest->content = (Byte *)malloc(sizeof(Byte) * dest->contentSize);
  memcpy(dest->content, src->content, dest->contentSize);
  strcpy(dest->bufferName, src->bufferName);
}

void destroyWorldCmd(WorldCmd *cmd)
{
  if (cmd->content) {
    free(cmd->content);
  }
}

void worldCmdExecute(WorldCmd *cmd, World *world)
{
  if (cmd->bufferName != NULL) {
    if (strlen(cmd->bufferName)
        && strcmp(cmd->bufferName, worldGetBufferName(world))) {
      worldSetCurrentBuffer(world, cmd->bufferName, 1);
    }
  }
  if (worldGetPoint(world) != cmd->position) {
    worldSetPoint(world, cmd->position);
  }
  worldCmds[cmd->type](cmd, world);
}

void worldCmdDestroy(WorldCmd *cmd, World *world)
{
  destroyWorld(world);
  worldNotifyObservers(world);
}

void worldCmdInsertExec(WorldCmd *cmd, World *world)
{
  worldInsert(world, cmd->content, cmd->size);
  worldNotifyObservers(world);
}

void worldCmdDeleteExec(WorldCmd *cmd, World *world)
{
  worldDelete(world, cmd->size);
  worldNotifyObservers(world);
}

void worldCmdMovePointForward(WorldCmd *cmd, World *world)
{
  worldMovePointForward(world, cmd->size);
  worldNotifyObservers(world);
}

void worldCmdMovePointBackward(WorldCmd *cmd, World *world)
{
  worldMovePointBackward(world, cmd->size);
  worldNotifyObservers(world);
}

void worldCmdSetPoint(WorldCmd *cmd, World *world)
{
  worldSetPoint(world, cmd->position);
  worldNotifyObservers(world);
}


void initWorldCmdList(WorldCmdList *cmdList, unsigned capacity)
{
  cmdList->list = (WorldCmd **)malloc(sizeof(WorldCmd *) * capacity);
  memset((void *)cmdList->list, 0, sizeof(WorldCmd *) * capacity);
  cmdList->capacity = capacity;
  cmdList->size = 0;
  cmdList->lock = NULL;
}

void destroyWorldCmdList(WorldCmdList *cmdList)
{
  free(cmdList->list);
  cmdList->list = NULL;
  cmdList->size = 0;
  cmdList->capacity = 0;
  worldCmdListSetShared(cmdList, 0);
}

/* #include "dbg/dbglog.h" */
WorldCmd *worldCmdListGet(WorldCmdList *cmdList, unsigned idx)
{
  return cmdList->list[idx % cmdList->capacity];

/* DBGLOG("worldCmdListGet", cmdList->list[((cmdList->first + seqNum) % cmdList->capacity)]->content, cmdList->list[((cmdList->first + seqNum) % cmdList->capacity)]->size); */
}

void worldCmdListPut(WorldCmdList *cmdList, WorldCmd *cmd)
{
  /* DBGLOG("worldCmdListPut", cmd->content, cmd->size); */

  unsigned idx;
  idx = cmdList->size % cmdList->capacity;
  if (cmdList->list[idx] != NULL) {
    destroyWorldCmd(cmdList->list[idx]);
    free(cmdList->list[idx]);
    cmdList->list[idx] = NULL;
  }
  cmdList->list[idx] = cmd;
  cmdList->size += 1;
}

void worldCmdListClear(WorldCmdList *cmdList)
{
  unsigned i;
  for (i = 0; i < cmdList->capacity; i++) {
    if (cmdList->list[i] != NULL) {
      destroyWorldCmd(cmdList->list[i]);
      cmdList->list[i] = NULL;
    }
  }
  cmdList->size = 0;
}

Payload *worldCmdListToPayload(WorldCmdList *cmdList)
{
  Size size;
  Payload *payload;
  int i;

  /* sums the sizes of the contents of each command */
  size = sizeof(WorldCmd) * cmdList->size;
  for (i = 0; i < cmdList->size; i++) {
    size += worldCmdListGet(cmdList, i)->contentSize;
  }

  payload = (Payload *)malloc(sizeof(Payload));
  payload->size = size;
  payload->data = malloc(payload->size);

  size = 0;
  for (i = 0; i < cmdList->size; i++) {
    /*There is going to be a problem if the BufferServer had spent
      too much time to send the commands so the SharingServer has
      already freed them */

    WorldCmd *currCmd;
    currCmd = worldCmdListGet(cmdList, i);

    memcpy(payload->data + size, currCmd, sizeof(WorldCmd));
    size += sizeof(WorldCmd);
    memcpy(payload->data + size, currCmd->content, currCmd->contentSize);

    size += currCmd->contentSize;	   
  }

  return payload;
}

void worldCmdListSetShared(WorldCmdList *cmdList, int isShared)
{
  if (isShared) {
    cmdList->lock = createWorldCmdListLock();
  }
  else {
    destroyWorldCmdListLock(cmdList->lock);
    cmdList->lock = NULL;
  }
}

int worldCmdListIsShared(WorldCmdList *cmdList)
{
  return (cmdList->lock != NULL);
}

void worldCmdListLockRead(WorldCmdList *cmdList)
{
  worldCmdListLockReadLock(cmdList->lock);
}

void worldCmdListUnlockRead(WorldCmdList *cmdList)
{
  worldCmdListLockReadUnlock(cmdList->lock);
}

void worldCmdListLockWrite(WorldCmdList *cmdList)
{
  worldCmdListLockWriteLock(cmdList->lock);
}

void worldCmdListUnlockWrite(WorldCmdList *cmdList)
{
  worldCmdListLockWriteUnlock(cmdList->lock);
}
