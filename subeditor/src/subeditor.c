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

#include <subeditor/def.h>
#include "subeditor-internal.h"
#include "buffer_commands.h"
#include "subeditor/world_observer.h"

#include <stdlib.h>
#include <assert.h>

#include <libsys/dbgdie.h>

const UInt32 WORLD_BUFFER_FLAG_EOB = BUFFER_FLAG_EOB;
const UInt32 WORLD_BUFFER_FLAG_BOB = BUFFER_FLAG_BOB;
const UInt32 WORLD_BUFFER_FLAG_MODIFIED = BUFFER_FLAG_MODIFIED;

void
initBufferChainNode(BufferChainNode *node, Buffer *buf,
		    BufferChainNode *prev, BufferChainNode *next)
{
  node->buf = buf;
  node->bufCmdStack = (BufferCmdStack *)malloc(sizeof(BufferCmdStack));
  initBufferCmdStack(node->bufCmdStack);
  node->prev = prev;
  node->next = next;
}

void destroyBufferChainNode(BufferChainNode *node)
{
  free(node->bufCmdStack);
}

BufferChainNode *
createBufferChainNode(Buffer *buf, BufferChainNode *prev, BufferChainNode *next)
{
  BufferChainNode *node;

  node = (BufferChainNode *)malloc(sizeof(BufferChainNode));
  initBufferChainNode(node, buf, prev, next);

  return node;
}

BufferChainNode *bufferChainFind(BufferChainNode *chain, char *name)
{
  BufferChainNode *match;

  match = chain;
  do {
    if (!strcmp(match->buf->name, name)) {
      return match;
    }
    match = match->next;
  } while (match != chain);

  return NULL;
}

World *createWorld(struct world_observer *observer)
{
  World *world;
  world = (World *)malloc(sizeof(World));
  initWorld(world, observer);
  return world;
}

void initWorld(World *world, struct world_observer *observer)
{
  Buffer *emptyBuf;

  emptyBuf = (Buffer *)malloc(sizeof(Buffer));
  initBuffer(emptyBuf, "*scratch*");

  world->bufferChain = createBufferChainNode(emptyBuf, NULL, NULL);
  world->bufferChain->prev = world->bufferChain->next = world->bufferChain;

  world->isAlive = 1;
  world->observer = observer;
}

void destroyWorld(World *world)
{
  BufferChainNode *node;

  while (world->bufferChain != NULL) {
    node = world->bufferChain;
    node->prev->next = NULL;
    world->bufferChain = world->bufferChain->next;
    destroyBuffer(node->buf);
    free(node);
  }

  world->bufferChain = NULL;
  world->isAlive = 0;
}

/*
void worldSave(World *world, char *filePath)
{
}

void worldLoad(World *world, char *filePath)
{
}
*/

void worldCreateBuffer(World *world, char *name)
{
  BufferChainNode *first;
  BufferChainNode *currNode;
  Buffer *buf;

  first = world->bufferChain;
  currNode = first;

  do {
    if (strcmp(currNode->buf->name, name)) {
      currNode = currNode->next;
    }
    else {
      die("TODO: world names must be unique.\n");
    }
  } while (currNode != first);

  buf = (Buffer *)malloc(sizeof(Buffer));
  initBuffer(buf, name);
  first->next = createBufferChainNode(buf, first, first->next);
  first->next->next->prev = first->next;
}

/*
void worldClearBuffer(World *world, char *name)
{
}
*/

void worldCloseBuffer(World *world)
{
  BufferChainNode *node;

  if (world->bufferChain->next == world->bufferChain) {
    die("The editor is designed to have always at least one loaded buffer.\n");
  }

  node = world->bufferChain;

  world->bufferChain = node->next;

  node->prev->next = node->next;
  node->next->prev = node->prev;

  destroyBufferChainNode(node);
  free(node);
}

void worldSetCurrentBuffer(World *world, char *name, short create)
{
  BufferChainNode *node;

  node = bufferChainFind(world->bufferChain, name);

  if (node != NULL) {
    world->bufferChain = node;
  }
  else if (create) {
    worldCreateBuffer(world, name);
    worldSetCurrentBuffer(world, name, 0);
  }
}

void worldNextBuffer(World *world)
{
  world->bufferChain = world->bufferChain->next;
}

void worldPrevBuffer(World *world)
{
  world->bufferChain = world->bufferChain->prev;
}

void worldInsert(World *world, Byte *src, Size size)
{
  BufferCommand *bufCmd;

  bufCmd = createBufferCommand(BUF_CMD_INSERT,
			       bufferGetPoint(world->bufferChain->buf),
			       size, src);
  bufferCommandExec(bufCmd, world->bufferChain->buf);

  bufferCmdStackPush(world->bufferChain->bufCmdStack, bufCmd);
}

void worldDelete(World *world, Size size)
{
  BufferCommand *bufCmd;

  bufCmd = createBufferCommand(BUF_CMD_DELETE,
			       bufferGetPoint(world->bufferChain->buf),
			       size, NULL);
  bufferCommandExec(bufCmd, world->bufferChain->buf);

  bufferCmdStackPush(world->bufferChain->bufCmdStack, bufCmd);
}

Size worldGetChunk(World *world, Byte *dest, Size size)
{
  /* TODO: usar o BufferCommand */
  return bufferGetChunk(world->bufferChain->buf, dest, size);
}

void worldMovePointForward(World *world, Size size)
{
  BufferCommand *bufCmd;
  bufCmd = createBufferCommand(BUF_CMD_MOVE_POINT_FORWARD,
                               bufferGetPoint(world->bufferChain->buf),
                               size, NULL);
  bufferCommandExec(bufCmd, world->bufferChain->buf);
  bufferCmdStackPush(world->bufferChain->bufCmdStack, bufCmd);
}

void worldMovePointBackward(World *world, Size size)
{
  BufferCommand *bufCmd;
  bufCmd = createBufferCommand(BUF_CMD_MOVE_POINT_BACKWARD,
                               bufferGetPoint(world->bufferChain->buf),
                               size, NULL);
  bufferCommandExec(bufCmd, world->bufferChain->buf);
  bufferCmdStackPush(world->bufferChain->bufCmdStack, bufCmd);
}

void worldSetPoint(World *world, Position bytePos)
{
  BufferCommand *bufCmd;
  bufCmd = createBufferCommand(BUF_CMD_SET_POINT, bytePos, 0, NULL);
  bufferCommandExec(bufCmd, world->bufferChain->buf);
  bufferCmdStackPush(world->bufferChain->bufCmdStack, bufCmd);
}

void worldAddMark(World *world, char *name)
{
  /* TODO: usar o BufferCommand */
  bufferAddMark(world->bufferChain->buf, name, 0);
}

void worldRemoveMark(World *world, char *name)
{
  /* TODO: usar o BufferCommand */
  bufferRemoveMark(world->bufferChain->buf, name);
}

void worldMarkToPoint(World *world, char *name)
{
  /* TODO: usar o BufferCommand */
  bufferMarkToPoint(world->bufferChain->buf, name);
}

void worldPointToMark(World *world, char *name)
{
  /* TODO: usar o BufferCommand */
  bufferPointToMark(world->bufferChain->buf, name);
}

short worldIsPointAtMark(World *world, char *name)
{
  return bufferIsPointAtMark(world->bufferChain->buf, name);
}

char *worldGetBufferFilePath(World *world)
{
  return bufferGetFilePath(world->bufferChain->buf);
}

void worldSetBufferFilePath(World *world, char *filePath)
{
  bufferSetFilePath(world->bufferChain->buf, filePath);
}

void worldReadBuffer(World *world)
{
  char *filePath;
  filePath = bufferGetFilePath(world->bufferChain->buf);

  assert(*filePath);

  bufferRead(world->bufferChain->buf);
}

void worldWriteBuffer(World *world)
{
  char *filePath;
  filePath = bufferGetFilePath(world->bufferChain->buf);

  assert(*filePath);

  bufferWrite(world->bufferChain->buf);
}

UInt32 worldGetBufferFlags(World *world)
{
  return bufferGetFlags(world->bufferChain->buf);
}

void worldSetBufferFlag(World *world, UInt32 flagMask, short isSet)
{
  if (isSet) {
    bufferSetFlags(world->bufferChain->buf, flagMask);
  }
  else {
    bufferResetFlags(world->bufferChain->buf, flagMask);
  }
}

void worldNotifyObservers(World *world)
{
  if (world->observer != NULL) {
    observerUpdate(world->observer, world);
  }
}

SUBEDITOR_INLINE_ short worldIsAlive(World *world)
{
  return world->isAlive;
}

SUBEDITOR_INLINE_ char *worldGetBufferName(World *world)
{
  return bufferGetName(world->bufferChain->buf);
}

SUBEDITOR_INLINE_ Size worldBufferSize(World *world)
{
  return bufferSize(world->bufferChain->buf);
}

SUBEDITOR_INLINE_ Position worldGetPoint(World *world)
{
  return bufferGetPoint(world->bufferChain->buf);
}

SUBEDITOR_INLINE_ short worldBufferExists(World *world, char *name)
{
  return (bufferChainFind(world->bufferChain, name) != NULL);
}
