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

#ifndef LIBSYS_SOCKET_H
#define LIBSYS_SOCKET_H

typedef struct payload {
  unsigned size;
  void *data;
} Payload;

typedef struct socket
{
  char *hostname;
  unsigned short port;

  int fd;

  /* TODO: better way to know if socket is listening/connected */
  short listening, connected;
} Socket;

/* hostname shall be NULL for listen sockets */
Socket *createSocket(char *hostname, unsigned short port);
/* hostname shall be NULL for listen sockets */
void initSocket(Socket *sock, char *hostname, unsigned short port);
void destroySocket(Socket *sock);
void socketListen(Socket *sock, unsigned listenQueueSize);
short socketConnect(Socket *sock);
Socket *socketAccept(Socket *listenSocket);
short socketIsListening(Socket *sock);
short socketIsConnected(Socket *sock);
void socketClose(Socket *sock);
char *socketGetHostname(Socket *sock);
#define socketGetHostname(sock) (((Socket *)sock)->hostname)
unsigned short socketGetPort(Socket *sock);
#define socketGetPort(sock) (((Socket *)sock)->port)
void socketSend(Socket *sock, Payload *payload);
/* payload must have it's content buffer allocated
   and its size set to it's maximum size.
   The function returns the number of bytes
   already read into the payload size. */
unsigned socketRecv(Socket *sock, Payload *payload, unsigned bytesRecvd);

#endif
