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

#include <sharing/buffer_server.h>

#include <pthread.h>
#include <stdlib.h>

BufferServer *createBufferServer(WorldCmdList *sharedWorldCmdList,
				 unsigned cmdSeqNum,
				 unsigned cmdBufferCapacity,
				 unsigned maxWaitToEmptyCmdBuffer,
				 World *world,
				 Socket *clSocket)
{
  BufferServer *sv;
  sv = (BufferServer *)malloc(sizeof(BufferServer));
  initBufferServer(sv, sharedWorldCmdList, cmdSeqNum, cmdBufferCapacity,
		   maxWaitToEmptyCmdBuffer, world, clSocket);
  return sv;
}


void initBufferServer(BufferServer *sv,
		      WorldCmdList *sharedWorldCmdList,
		      unsigned cmdSeqNum,
		      unsigned cmdBufferCapacity,
		      unsigned maxWaitToEmptyCmdBuffer,
		      World *world,
		      Socket *clSocket)
{
  sv->sharedWorldCmdList = sharedWorldCmdList;
  sv->cmdSeqNum = cmdSeqNum;
  sv->cmdBufferCapacity = cmdBufferCapacity;
  sv->maxWaitToEmptyCmdBuffer = maxWaitToEmptyCmdBuffer;
  sv->world = world;
  sv->clSocket = clSocket;
  sv->cmdBuffer = (WorldCmdList *)malloc(sizeof(WorldCmdList));
  initWorldCmdList(sv->cmdBuffer, 5);
}


void *runbs(void *sv) {
  WorldCmd *currCmd;
  WorldCmd *cmdCopy;
  Payload payload;
  time_t lastSent;
  int isCmdListFull;
  int loop;
  struct timespec nsecs;
  nsecs.tv_nsec = 50000000;

  time(&lastSent);
  isCmdListFull = 0;
  while (1) {
    loop = 1;
    worldCmdListLockRead(((BufferServer *)sv)->sharedWorldCmdList);
    do {
      currCmd = worldCmdListGet(((BufferServer *)sv)->sharedWorldCmdList,
                                ((BufferServer *)sv)->cmdSeqNum);
      if (currCmd && currCmd->seqNum == ((BufferServer *)sv)->cmdSeqNum) {
        /* It is a new command */
        if (!((BufferServer *)sv)->clSocket) {
          /* Does nothing because it is the main
             thread that should operate on local buffers.
             Otherwise it should be something like the following: */
          /* worldCmdExecute(currCmd, ((BufferServer *)sv)->world); */
        }
        else {
          cmdCopy = (WorldCmd *)malloc(sizeof(WorldCmd));
          copyWorldCmd(cmdCopy, currCmd);
          worldCmdListPut(((BufferServer *)sv)->cmdBuffer, cmdCopy);
          if (worldCmdListGetSize(((BufferServer *)sv)->cmdBuffer)
              >= ((BufferServer *)sv)->cmdBufferCapacity) {
            /* cmd list is full */
            isCmdListFull = 1;
          }
        }
        if (cmdCopy->type == WORLDCMD_DIE) {
          break;
        }
        ((BufferServer *)sv)->cmdSeqNum += 1;
      }
      else {
        loop = 0;
      }
      /* #include "dbg/dbglog.h" */
      /* char a = worldCmdListGetSize(((BufferServer *)sv)->cmdBuffer) + '0'; */
      /* DBGLOG("BufferServer", &a, 1); */
      if (isCmdListFull || difftime(time(0), lastSent)
          >= ((BufferServer *)sv)->maxWaitToEmptyCmdBuffer) {
        /* If the cmd buffer has run out of space
           or if there's been a silence */
        if (worldCmdListGetSize(((BufferServer *)sv)->cmdBuffer)) {
          /* send the commands if any */
          socketSend(((BufferServer *)sv)->clSocket,
                     worldCmdListToPayload(((BufferServer *)sv)->cmdBuffer));
          worldCmdListClear(((BufferServer *)sv)->cmdBuffer);
          time(&lastSent);
          isCmdListFull = 0;
        }
      }
    } while (loop);
    worldCmdListUnlockRead(((BufferServer *)sv)->sharedWorldCmdList);
    nanosleep(&nsecs, NULL);
  }

  return NULL;
}

void bufferServerRun(BufferServer *server)
{
  pthread_t tid;
  pthread_create(&tid, NULL, runbs, (void *)server);
}

void bufferServerProcessWorldCmd(BufferServer *server, WorldCmd *cmd)
{
  worldCmdExecute(cmd, server->world);
}

int bufferServerIsLocal(BufferServer *server)
{
  return (server->clSocket == NULL);
}

void destroyBufferServer(BufferServer *server)
{
}
