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

#ifndef BUFFER_SERVER_H
#define BUFFER_SERVER_H

#include <subeditor.h>
#include "worldcmd.h"
#include <libsys/socket.h>

typedef struct buffer_server
{
  /* A list of the world commands published by the sharing server */
  WorldCmdList *sharedWorldCmdList;
  unsigned cmdSeqNum;

  /* Buffer to store world commands before sending.
   Sending occours when cmdBuffer gets full or when
  maxWaitToEmptyCmdBuffer is reached.*/
  WorldCmdList *cmdBuffer;
  unsigned cmdBufferCapacity;
  unsigned maxWaitToEmptyCmdBuffer; /* measured in ms */
  
  World *world;

  /* NULL if local */
  Socket *clSocket;
} BufferServer;

BufferServer *createBufferServer(WorldCmdList *sharedWorldCmdList,
				 unsigned cmdSeqNum,
				 unsigned cmdBufferCapacity,
				 unsigned maxWaitToEmptyCmdBuffer,
				 World *world,
				 Socket *clSocket);

void initBufferServer(BufferServer *,
		      WorldCmdList *sharedWorldCmdList,
		      unsigned cmdSeqNum,
		      unsigned cmdBufferCapacity,
		      unsigned maxWaitToEmptyCmdBuffer,
		      World *world,
		      Socket *clSocket);

void bufferServerRun(BufferServer *);
void bufferServerProcessWorldCmd(BufferServer *, WorldCmd *);
int bufferServerIsLocal(BufferServer *);

void destroyBufferServer(BufferServer *);

#endif
