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

#ifndef SHARING_SERVER_H
#define SHARING_SERVER_H

#include "worldcmd.h"
#include <libsys/socket.h>
#include "buffer_server.h"
#include <pthread.h>

/* TODO: increase it. It's small for testing purposes */
#define SHARING_DEFAULT_CMD_CAP 24

typedef struct buffer_server_chain_node
{
  BufferServer *bufferServer;
  struct buffer_server_chain_node *next;
} BufferServerChainNode;

typedef struct buffer_server_chain
{
  BufferServerChainNode *head;
} BufferServerChain;

typedef struct sharing_server
{
  unsigned currSeqNum;
  /* WorldCommands to be multicasted will be pushed into this list */
  WorldCmdList *sharedWorldCmdList;
  Socket *listenSocket;
  World *world;
  BufferServerChain *bufSvChain;
  int port;
} SharingServer;

void initSharingServer(SharingServer *, World *, unsigned short port);
void destroySharingServer(SharingServer *);

void sharingServerCreateBufferServer(SharingServer *, Socket *);

void sharingServerMulticast(SharingServer *, WorldCmd *);

/* Listen a socket for new connections */
void sharingServerListen(SharingServer *);
void sharingServerStopListening(SharingServer *);
int sharingServerIsListening(SharingServer *);
void sharingServerSetPort(SharingServer *, int);

/* Connects to a listening SharingServer */
/* TODO: maybe change this to sharingServerAddNode()
   aiming to make it a distributed service or even p2p */
void sharingServerConnect(SharingServer *, Socket *);

void sharingServerShareBuffer(SharingServer *);

#endif
