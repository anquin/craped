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

#ifndef WORLD_H
#define WORLD_H

#include <subeditor/def.h>

#define WORLD_MAX_BUFFER_NAME BUFFER_NAME_MAX

typedef struct world World;
struct world_observer;

/* Inits the world with an empty buffer */
World *createWorld(struct world_observer *observer);
void initWorld(World *world, struct world_observer *observer);

void destroyWorld(World *world);
void worldSave(World *world, char *filePath);
void worldLoad(World *world, char *filePath);
void worldCreateBuffer(World *world, char *name);
void worldClearBuffer(World *world, char *name);
void worldCloseBuffer(World *world);
void worldSetCurrentBuffer(World *world, char *name, short create);
void worldNextBuffer(World *world);
void worldPrevBuffer(World *world);
void worldInsert(World *world, Byte *src, Size size);
void worldDelete(World *world, Size size);
Size worldGetChunk(World *world, Byte *dest, Size size);
void worldMovePointForward(World *world, Size size);
void worldMovePointBackward(World *world, Size size);
void worldSetPoint(World *world, Position bytePos);
Position worldGetPoint(World *world);
void worldAddMark(World *world, char *name);
void worldRemoveMark(World *world, char *name);
void worldMarkToPoint(World *world, char *name);
void worldPointToMark(World *world, char *name);
short worldIsPointAtMark(World *world, char *name);
char *worldGetBufferFilePath(World *world);
void worldSetBufferFilePath(World *world, char *filePath);
void worldReadBuffer(World *world);
void worldWriteBuffer(World *world);
UInt32 worldGetBufferFlags(World *world);
void worldSetBufferFlag(World *world, UInt32 flagMask, short isSet);
void worldNotifyObservers(World *world);
SUBEDITOR_INLINE_ short worldIsAlive(World *world);
SUBEDITOR_INLINE_ char *worldGetBufferName(World *world);
SUBEDITOR_INLINE_ Size worldBufferSize(World *world);
SUBEDITOR_INLINE_ Position worldGetPoint(World *world);

#endif
