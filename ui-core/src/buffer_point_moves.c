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

#include <uicore/buffer_point_moves.h>

void asciiMovePoint(World *world, int displ)
{
  if (!displ) return;

  worldMovePoint(world, displ);
}

void utf8MovePoint(World *world, int displ)
{
  Byte ch;
  int unit;

  if (!displ) return;

  unit = (displ < 0) ? -1 : 1;

  do {
    worldMovePoint(world, unit);
    if (worldBufferSize(world) == worldGetPoint(world)) {
      break;
    }
    worldGetChunk(world, &ch, 1);
  } while ((ch & 0xC0) == 0x80);
}

void utf8WordMovePoint(World *world, int displ)
{
  Byte ch;
  int unit;

  if (!displ) return;

  unit = (displ < 0) ? -1 : 1;

  do {
    utf8MovePoint(world, unit);
    if (worldBufferSize(world) == worldGetPoint(world)) {
      break;
    }
    worldGetChunk(world, &ch, 1);
  } while (worldGetPoint(world) != 0 && ch != ' ' && ch != '\n');
}

void utf8LineMovePoint(World *world, int displ)
{
  Byte ch;
  int unit;
  short stop;
  short i;

  if (!displ) return;

  unit = (displ < 0) ? -1 : 1;

  stop = 1;
  for (i = 0; i < 2; i++) {
    do {
      do {
        if (worldGetPoint(world) == 0 && displ < 0) {
          stop = 0;
          break;
        }
        worldMovePoint(world, unit);
        if (worldBufferSize(world) == worldGetPoint(world)) {
          stop = 0;
          break;
        }
        worldGetChunk(world, &ch, 1);
      } while ((ch & 0xC0) == 0x80);
    } while (ch != '\n' && stop);
    if (displ > 0 || i) {
      if (worldGetPoint(world)
          && (worldBufferSize(world) - 1) > worldGetPoint(world)) {
        utf8MovePoint(world, 1);
      }
      break;
    }
  }

  utf8LineMovePoint(world, (displ > 0) ? --displ : ++displ);
}
