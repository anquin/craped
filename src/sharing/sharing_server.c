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

#include <sharing/sharing_server.h>
#include <sharing/buffer_server.h>
#include <stdlib.h>
/* #include <stdio.h> */
#include <assert.h>
#include <libsys/dbglog.h>

void sharingServerShareBufferSpecific(SharingServer *server,
                                      WorldCmdList *worldCmdList);

void initBufferServerChain(BufferServerChain *bufSvChain)
{
  bufSvChain->head = NULL;
}

void destroyBufferServerChain(BufferServerChain *bufSvChain)
{
  BufferServerChainNode *bufSvChainNode;

  while (bufSvChain->head != NULL) {
    bufSvChainNode = bufSvChain->head;
    bufSvChain->head = bufSvChain->head->next;
    free(bufSvChainNode);
  }
}

void bufferServerChainAdd(BufferServerChain *bufSvChain, BufferServer *bufSv)
{
  BufferServerChainNode *bufSvChainNode=
    (BufferServerChainNode *)malloc(sizeof(BufferServerChainNode));
  bufSvChainNode->bufferServer = bufSv;
  bufSvChainNode->next = bufSvChain->head;
  bufSvChain->head = bufSvChainNode;
}

void initSharingServer(SharingServer *server, World *world, unsigned short port)
{
  server->currSeqNum = 0;
  server->sharedWorldCmdList = (WorldCmdList *)malloc(sizeof(WorldCmdList));
  initWorldCmdList(server->sharedWorldCmdList, SHARING_DEFAULT_CMD_CAP);
  worldCmdListSetShared(server->sharedWorldCmdList, 1);
  server->port = port;
  server->world = world;
  server->bufSvChain = (BufferServerChain *)malloc(sizeof(BufferServerChain));
  initBufferServerChain(server->bufSvChain);
}

void destroySharingServer(SharingServer *server)
{
  destroyWorldCmdList(server->sharedWorldCmdList);
  if (server->listenSocket != NULL) {
    destroySocket(server->listenSocket);
  }
  free(server->sharedWorldCmdList);
  free(server->listenSocket);
  destroyBufferServerChain(server->bufSvChain);
  free(server->bufSvChain);
}

void sharingServerCreateBufferServer(SharingServer *server, Socket *clSocket)
{
  BufferServer *bufSv;

  bufSv = createBufferServer(server->sharedWorldCmdList,
			     server->currSeqNum,
			     5,
			     2,
			     server->world,
			     clSocket);

  if (clSocket) {
    /* Sends the current content to the new client if it is remote */
    /* TODO: send all shared buffers */
    sharingServerShareBufferSpecific(server, bufSv->cmdBuffer);
    /* Remote BufferServers run as different threads */
    bufferServerRun(bufSv);
  }
  /* Local BufferServers have its methods invoked by the main thread */
  bufferServerChainAdd(server->bufSvChain, bufSv);
}

void sharingServerMulticast(SharingServer *server, WorldCmd *cmd)
{
/* #include "dbg/dbglog.h" */
/*   DBGLOG("sharingServerMulticast", cmd->content, cmd->size); */

  BufferServerChainNode *bufSvChainNode;

  /* For remote buffer servers store the commands in the shared list */
  worldCmdListLockWrite(server->sharedWorldCmdList);
  cmd->seqNum = server->currSeqNum++;
  worldCmdListPut(server->sharedWorldCmdList, cmd);
  worldCmdListUnlockWrite(server->sharedWorldCmdList);

  /* For local buffer servers use the local chain */
  bufSvChainNode = server->bufSvChain->head;
  while (bufSvChainNode != NULL) {
    if (bufferServerIsLocal(bufSvChainNode->bufferServer)) {
      bufferServerProcessWorldCmd(bufSvChainNode->bufferServer, cmd);
    }
    bufSvChainNode = bufSvChainNode->next;
  }
}

void *runss(void *sv) {
  Socket *clSocket;

  while (socketIsListening(((SharingServer *)sv)->listenSocket)) {
    clSocket = socketAccept(((SharingServer *)sv)->listenSocket);
    assert(clSocket != NULL);
    sharingServerCreateBufferServer(((SharingServer *)sv), clSocket);
  }

  return NULL;
}

/* Listen a socket for new connections */
void sharingServerListen(SharingServer *server)
{
  pthread_t tid;

  server->listenSocket =
    createSocket(NULL, server->port);
  socketListen(server->listenSocket, 10);
  pthread_create(&tid, NULL, runss, (void *)server);
}

void sharingServerStopListening(SharingServer *server)
{
  /* TODO */
}

int sharingServerIsListening(SharingServer *server)
{
  return (server->listenSocket == NULL)
    ? 0 : socketIsListening(server->listenSocket);
}

void sharingServerSetPort(SharingServer *server, int port)
{
  server->port = port;
}

struct sc_params {
  Socket *socket;
  SharingServer *sharingServer;
};

void *runsc(void *params)
{
  Payload payload;
  WorldCmd *currCmd;
  unsigned nbytes;

  do {
    payload.size = sizeof(WorldCmd);
    payload.data = malloc(payload.size);
    nbytes = 0;
    do {
      nbytes = socketRecv(((struct sc_params *)params)->socket, &payload, nbytes);
    } while(nbytes < payload.size);
    currCmd = (WorldCmd *)payload.data;
    payload.size = currCmd->contentSize;
    payload.data = malloc(payload.size);
    nbytes = 0;
    do {
      nbytes = socketRecv(((struct sc_params *)params)->socket, &payload, nbytes);
    } while(nbytes < payload.size);
    currCmd->content = payload.data;
    sharingServerMulticast(((struct sc_params *)params)->sharingServer,
			   currCmd);
  } while (1);

  socketClose(((struct sc_params *)params)->socket);

  return NULL;
}

void sharingServerConnect(SharingServer *server, Socket *socket)
{
  pthread_t tid;
  struct sc_params *params;

  params = (struct sc_params *)malloc(sizeof(struct sc_params));
  params->socket = socket;
  params->sharingServer = server;

  assert(socketConnect(socket));
  pthread_create(&tid, NULL, runsc, params);
}

WorldCmdList *sharingServerShareBufferCreateCommands(SharingServer *server)
{
  Byte *content;
  Size bufSize;
  WorldCmdList *commands;

  commands = (WorldCmdList *)malloc(sizeof(WorldCmdList));
  /* One command is enough for now */
  initWorldCmdList(commands, 1);
  bufSize = worldBufferSize(server->world);
  content = (Byte *)malloc(sizeof(Byte) * bufSize);
  worldAddMark(server->world, "*tmp");
  worldSetPoint(server->world, 0);
  worldGetChunk(server->world, content, bufSize);
  worldMarkToPoint(server->world, "*tmp");
  worldRemoveMark(server->world, "*tmp");

  worldCmdListPut(commands, createWorldCmd(WORLDCMD_INSERT, 0, bufSize,
                                           bufSize, content,
                                           worldGetBufferName(server->world)));
  return commands;
}

void sharingServerShareBufferSpecific(SharingServer *server,
                                      WorldCmdList *worldCmdList)
{
  unsigned i;
  unsigned listSz;
  WorldCmd *cmd;
  WorldCmdList *cmdList;
  cmdList = sharingServerShareBufferCreateCommands(server);
  listSz = worldCmdListGetSize(cmdList);
  for (i = 0; i < listSz; i++) {
    cmd = worldCmdListGet(cmdList, i);
    worldCmdListPut(worldCmdList, cmd);
  }
}

void sharingServerShareBuffer(SharingServer *server)
{
  unsigned i;
  unsigned listSz;
  WorldCmd *cmd;
  WorldCmdList *cmdList;
  cmdList = sharingServerShareBufferCreateCommands(server);
  listSz = worldCmdListGetSize(cmdList);
  for (i = 0; i < listSz; i++) {
    cmd = worldCmdListGet(cmdList, i);
    worldCmdListLockWrite(server->sharedWorldCmdList);
    cmd->seqNum = server->currSeqNum++;
    worldCmdListPut(server->sharedWorldCmdList, cmd);
    worldCmdListUnlockWrite(server->sharedWorldCmdList);
  }
}
