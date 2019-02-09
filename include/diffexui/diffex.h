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

#ifndef DIFFEX_H
#define DIFFEX_H

#include <uicore/ui.h>

#include <stdlib.h>

#define DE_ERASE 1
#define DE_SHOW 2
#define DE_UPDATE 3

/* For debugging */
extern char *de_modes[];

typedef struct de_fifo DEFifo;

typedef struct diffex
{
  int mode;
  DEFifo *fifo;
} Diffex;

void initDiffex(Diffex *diffex, unsigned fifoCapacity);
void destroyDiffex(Diffex *diffex);
void diffexView(Diffex *diffex, UI *ui, World *world);
#define diffexGetMode(diffex) (((Diffex *)diffex)->mode)
#define diffexSetMode(diffex, m) (((Diffex *)diffex)->mode = m)

#define X(expr) (diffex->mode != DE_ERASE ? (expr) : 0)
#define IF(b) {int m1 = diffex->mode; if (ifutil(diffex, X(b) && 1)) {
#define FOR(a, b, c) {int m1 = diffex->mode; for (X(a);ifutil(diffex, X(b) && 1); X(c)) {
#define WHILE(b) {int m1 = diffex->mode; while (ifutil(diffex, X(b) && 1)) {
#define DO {int m1 = diffex->mode; do {
#define END_WHILE(b) } while (ifutil(diffex, X(b) && 1)); diffex->mode = m1;}
#define END } diffex->mode = m1; }
#define CALL(f, args) DBGLOG("CALL(%s): %s%s\n", de_modes[diffex->mode - 1], #f, #args); f args

int ifutil(Diffex *diffex, int b);

#endif
