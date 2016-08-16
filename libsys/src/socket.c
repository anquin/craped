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

#include <libsys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#include "libsys/dbglog.h"

Socket *createSocket(char *hostname, unsigned short port)
{
  Socket *socket;

  socket = (Socket *)malloc(sizeof(Socket));
  if (!socket) {
    return socket;
  }
  initSocket(socket, hostname, port);

  return socket;
}

void initSocket(Socket *sock, char *hostname, unsigned short port)
{
  sock->hostname = hostname;
  sock->port = port;

  sock->fd = 0;

  sock->connected = 0;
  sock->listening = 0;
}

void destroySocket(Socket *sock)
{
  if (sock->connected || sock->listening) {
    socketClose(sock);
  }
}

void socketListen(Socket *sock, unsigned listenQueueSize)
{
  struct sockaddr_in svData;
  int socketfd;

  bzero(&svData, sizeof(svData));
  svData.sin_family = AF_INET;
  svData.sin_addr.s_addr = htonl(INADDR_ANY);
  svData.sin_port = htons(sock->port);

  socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (bind(socketfd, (struct sockaddr *)&svData, sizeof(svData)) < 0) {
    fprintf(stderr, "ERROR: unable to bind socket.\n");;
    close(socketfd);
    exit(0);
  }

  if (listen(socketfd, listenQueueSize) < 0) {
    fprintf(stderr, "ERROR: unable to listen.\n");
    close(socketfd);
    exit(0);
  }

  sock->fd = socketfd;
  sock->listening = 1;
}

Socket *socketAccept(Socket *listenSocket)
{
  int clDataLen;
  struct sockaddr_in clData;
  int aSocket;
  char *clAddr;
  int clPort;
  Socket *clSocket;

  aSocket = accept(listenSocket->fd,
		   (struct sockaddr *)&clData,
		   (socklen_t *)&clDataLen);
  if (aSocket < 0) {
    int errnum = errno;
    DBGLOG("ERROR: %d socket accept() -> %s\n", errnum, strerror(errnum));
    return NULL;
  }

  clAddr = inet_ntoa(clData.sin_addr);
  clPort = ntohs(clData.sin_port);

  DBGLOG("Accepted connection from: %s:%d\n", clAddr, clPort);

  clSocket = (Socket *)malloc(sizeof(Socket));
  initSocket(clSocket, clAddr, clPort);
  clSocket->fd = aSocket;
  clSocket->connected = 1;

  return clSocket;
}

short socketIsListening(Socket *sock)
{
  return sock->listening;
}

short socketIsConnected(Socket *sock)
{
  return sock->connected;
}

short socketConnect(Socket *sock)
{
  struct sockaddr_in clData;
  int sockfd;
  int result;

  DBGLOG("Connecting to server %s:%d...\n",
         socketGetHostname(sock), socketGetPort(sock));

  sockfd = 0;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "Error: Could not create socket \n");
    exit(1);
  }

  bzero(&clData, sizeof(clData));
  clData.sin_family = AF_INET;
  clData.sin_port = htons(sock->port);

  result = inet_pton(AF_INET, socketGetHostname(sock), &(clData.sin_addr));
  if (result <= 0) {
    fprintf(stderr, "ERROR: inet_pton error %d occured.\n", result);
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr *)&clData, sizeof(clData)) < 0) {
    return 0;
  }

  sock->fd = sockfd;
  sock->connected = 1;

  return 1;
}

void socketSend(Socket *sock, Payload *payload)
{
  send(sock->fd, payload->data, payload->size, 0);
}

unsigned socketRecv(Socket *sock, Payload *payload, unsigned bytesRecvd)
{
  bytesRecvd += recv(sock->fd, payload->data, payload->size - bytesRecvd, 0);
  return bytesRecvd;
}

void socketClose(Socket *sock)
{
  if (close(sock->fd)) {
    fprintf(stderr, "Unable to close socket.\n");
  }
  sock->listening = 0;
  sock->connected = 0;
}
