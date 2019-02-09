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

#include <sharing/worldcmd_list_lock.h>

#include <stdlib.h>

WorldCmdListLock *createWorldCmdListLock(void)
{
  WorldCmdListLock *lock;
  lock = (WorldCmdListLock *)malloc(sizeof(WorldCmdListLock));
  initWorldCmdListLock(lock);
  return lock;
}

void initWorldCmdListLock(WorldCmdListLock *lock)
{
  lock->mut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  lock->writeOK = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  lock->readOK = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));

  pthread_mutex_init(lock->mut, NULL);
  pthread_cond_init(lock->writeOK, NULL);
  pthread_cond_init(lock->readOK, NULL);
  lock->readers = 0;
  lock->writers = 0;
  lock->waiting = 0;
}

void destroyWorldCmdListLock(WorldCmdListLock *lock)
{
  pthread_mutex_destroy(lock->mut);
  pthread_cond_destroy(lock->readOK);
  pthread_cond_destroy(lock->writeOK);
  free(lock);
  free(lock->readOK);
  free(lock->writeOK);
}


void worldCmdListLockWriteLock(WorldCmdListLock *lock)
{
  pthread_mutex_lock (lock->mut);
  lock->waiting++;
  while (lock->readers || lock->writers) {
    pthread_cond_wait(lock->writeOK, lock->mut);
  }
  lock->waiting--;
  lock->writers++;
  pthread_mutex_unlock(lock->mut);
}

void worldCmdListLockWriteUnlock(WorldCmdListLock *lock)
{
  pthread_mutex_lock(lock->mut);
  lock->writers--;
  pthread_cond_broadcast(lock->readOK);
  pthread_mutex_unlock(lock->mut);
}

void worldCmdListLockReadLock(WorldCmdListLock *lock)
{
  pthread_mutex_lock (lock->mut);
  if (lock->writers || lock->waiting) {
    do {
      pthread_cond_wait(lock->readOK, lock->mut);
    } while (lock->writers);
  }
  lock->readers++;
  pthread_mutex_unlock(lock->mut);
}

void worldCmdListLockReadUnlock(WorldCmdListLock *lock)
{
  pthread_mutex_lock(lock->mut);
  lock->readers--;
  pthread_cond_signal(lock->writeOK);
  pthread_mutex_unlock(lock->mut);
}
