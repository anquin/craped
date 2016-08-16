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

#ifndef WORLDCMD_LIST_LOCK_H
#define WORLDCMD_LIST_LOCK_H

#include <pthread.h>

typedef struct world_cmd_list_lock
{
  pthread_mutex_t *mut;
  int writers;
  int readers;
  int waiting;
  pthread_cond_t *writeOK;
  pthread_cond_t *readOK;
} WorldCmdListLock;

WorldCmdListLock *createWorldCmdListLock(void);
void initWorldCmdListLock(WorldCmdListLock *);
void destroyWorldCmdListLock(WorldCmdListLock *);

void worldCmdListLockWriteLock(WorldCmdListLock *);
void worldCmdListLockWriteUnlock(WorldCmdListLock *);
void worldCmdListLockReadLock(WorldCmdListLock *);
void worldCmdListLockReadUnlock(WorldCmdListLock *);

#endif
