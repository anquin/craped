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

#ifndef WORLDCMD_LIST_H
#define WORLDCMD_LIST_H

#include <subeditor.h>
#include <libsys/socket.h>
#include "worldcmd_list_lock.h"

#define WORLDCMD_DIE                 0
#define WORLDCMD_INSERT              1
#define WORLDCMD_DELETE              2
#define WORLDCMD_MOVE_POINT_FORWARD  3
#define WORLDCMD_MOVE_POINT_BACKWARD 4
#define WORLDCMD_SET_POINT           5

typedef unsigned WorldCmdType;

typedef struct world_cmd
{
  unsigned seqNum;
  WorldCmdType type;
  Position position;
  Size size;
  Size contentSize;
  Byte *content;
  char bufferName[WORLD_MAX_BUFFER_NAME];
} WorldCmd;


WorldCmd *
createWorldCmd(WorldCmdType type,
	       Position position,
	       Size size,
	       Size contentSize,
	       Byte *content,
               char *bufferName);
void
initWorldCmd(WorldCmd *cmd,
	     WorldCmdType type,
	     Position position,
	     Size size,
	     Size contentSize,
	     Byte *content,
             char *bufferName);

void copyWorldCmd(WorldCmd *dest, WorldCmd *src);
void destroyWorldCmd(WorldCmd *cmd);
#define worldCmdGetBufferName(cmd) (((WorldCmd *)cmd)->bufferName)

void worldCmdExecute(WorldCmd *, World *);

void worldCmdInsertExec(WorldCmd *, World *);
void worldCmdDeleteExec(WorldCmd *, World *);
void worldCmdMovePointForward(WorldCmd *, World *);
void worldCmdMovePointBackward(WorldCmd *, World *);
void worldCmdSetPoint(WorldCmd *, World *);
void worldCmdDestroy(WorldCmd *, World *);

typedef void (*WorldCmdExecuteFn)(WorldCmd *, World *);

extern WorldCmdExecuteFn worldCmds[];

/* Defines a circular list of world commands used for publishing */

typedef struct world_cmd_list
{
  unsigned capacity;
  unsigned size;
  WorldCmd **list;

  /* Used in case the list is shared */
  WorldCmdListLock *lock;
} WorldCmdList;

void initWorldCmdList(WorldCmdList *, unsigned capacity);
void destroyWorldCmdList(WorldCmdList *);

/* Returns NULL if the command is not there anymore */
WorldCmd *worldCmdListGet(WorldCmdList *, unsigned idx);
void worldCmdListPut(WorldCmdList *, WorldCmd *);
unsigned worldCmdListGetSize(WorldCmdList *);
#define worldCmdListGetSize(list) (((WorldCmdList *)list)->size)
void worldCmdListClear(WorldCmdList *);
Payload *worldCmdListToPayload(WorldCmdList *);

/* Receives nonzero if it is shared */
void worldCmdListSetShared(WorldCmdList *, int);
int worldCmdListIsShared(WorldCmdList *);

void worldCmdListLockWrite(WorldCmdList *cmdList);
void worldCmdListUnlockWrite(WorldCmdList *cmdList);
void worldCmdListLockRead(WorldCmdList *cmdList);
void worldCmdListUnlockRead(WorldCmdList *cmdList);

#endif
