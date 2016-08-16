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

#include "uicore/framer.h"

#include <subeditor/mode.h>

void framer(Window *window, Terminal *term, World *world)
{
  Position newStartPos;
  Size cnt;
  MoveBufferPointFn moveBufferPointFn;
  MoveBufferPointFn moveLineBufferPointFn;
  unsigned termHeight;
  unsigned wndHeight;
  Mode *mode;
  mode = windowGetMode(window);

  moveBufferPointFn = modeGetBufferPointFn(mode);
  moveLineBufferPointFn = modeGetBufferPointLineFn(mode);
  termHeight = terminalGetHeight(term);
  wndHeight = termHeight - ((windowHasStatusLine(window)) ? 0 : 1);

  worldAddMark(world, "saved");

  newStartPos = 0;
  if (worldGetPoint(world)) {
    moveLineBufferPointFn(world, -1);
  }
  for (cnt = 0; cnt < wndHeight; cnt++) {
    if (worldGetPoint(world) == windowGetTop(window)) {
      break;
    }

    if (!worldGetPoint(world)) {
      break;
    }

    if (cnt == wndHeight / 2) {
      newStartPos = worldGetPoint(world);
    }

    moveLineBufferPointFn(world, -1);
  }

  if (cnt >= wndHeight) {
    windowSetTop(window, newStartPos);
    worldSetPoint(world, newStartPos);
    for (cnt = 0; cnt < wndHeight; cnt++) {
      if (worldBufferSize(world) == worldGetPoint(world)) break;
      moveLineBufferPointFn(world, 1);moveBufferPointFn(world, 1);
    }
    windowSetContentSize(window, worldGetPoint(world) - newStartPos);
  }
  else if (!worldGetPoint(world)) {
    windowSetTop(window, 0);
  }

  worldMarkToPoint(world, "saved");
  worldRemoveMark(world, "saved");
}
