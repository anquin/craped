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

#include <libsys/dbglog.h>

#include <uicore/windowmanager.h>
#include <assert.h>
#include <stdlib.h>

#define BUF_NAME "buffer1"

unsigned terminalGetHeight(struct terminal *t) {}
void observerUpdate(struct world_observer *wo, struct world *w) {}

int test1(void);
int windowChainTest(void);

int main(void)
{
  int result;

  DBGLOG_OPEN(stdout);

  result =
    windowChainTest()
    || test1();

  DBGLOG_CLOSE();

  return result;
}

int test1(void)
{
  WindowManager wndMan;
  Window *wnd1, *wnd2, *wnd3, *wnd4;
  unsigned wnd1PosX, wnd1PosY, wnd2PosX, wnd2PosY, wnd3PosX, wnd3PosY;
  unsigned wnd1SizeX, wnd1SizeY, wnd2SizeX, wnd2SizeY, wnd3SizeX, wnd3SizeY;

  initWindowManager(&wndMan);

  wnd1 = windowManagerGetWindow(&wndMan);
  windowSetBufferName(wnd1, BUF_NAME);
  windowManagerSplitWindow(&wndMan, HORIZONTAL);
  windowManagerNextWindow(&wndMan);
  wnd2 = windowManagerGetWindow(&wndMan);

  windowGetPosition(wnd1, &wnd1PosX, &wnd1PosY);
  windowGetPosition(wnd2, &wnd2PosX, &wnd2PosY);
  windowGetSize(wnd1, &wnd1SizeX, &wnd1SizeY);
  windowGetSize(wnd2, &wnd2SizeX, &wnd2SizeY);

  assert(wnd1 != wnd2);
  assert(!strcmp(windowGetBufferName(wnd1), windowGetBufferName(wnd2)));
  assert(wnd1PosX == 0);assert(wnd1PosY == 0);
  assert(wnd2PosX == 0);assert(wnd2PosY == WNDMAN_MAX_SPLITS_H / 2);
  assert(wnd1SizeX == WNDMAN_MAX_SPLITS_V);
  assert(wnd1SizeY == WNDMAN_MAX_SPLITS_H / 2);
  assert(wnd1SizeX == wnd2SizeX);
  assert(wnd1SizeY == wnd2SizeY);

  windowManagerPrevWindow(&wndMan);
  wnd3 = windowManagerGetWindow(&wndMan);
  windowManagerPrevWindow(&wndMan);
  wnd4 = windowManagerGetWindow(&wndMan);
  assert(wnd1 == wnd3);assert(wnd2 == wnd4);

  windowManagerNextWindow(&wndMan);
  wnd3 = windowManagerGetWindow(&wndMan);
  windowManagerNextWindow(&wndMan);
  wnd4 = windowManagerGetWindow(&wndMan);
  assert(wnd1 == wnd3);assert(wnd2 == wnd4);

  windowManagerNextWindow(&wndMan);  /* Goes to the first window */
  assert(wnd1 == windowManagerGetWindow(&wndMan));

  windowManagerSplitWindow(&wndMan, HORIZONTAL);
  windowManagerNextWindow(&wndMan);
  wnd3 = windowManagerGetWindow(&wndMan);
  assert(wnd1 != wnd3);
  windowGetPosition(wnd1, &wnd1PosX, &wnd1PosY);
  windowGetSize(wnd1, &wnd1SizeX, &wnd1SizeY);
  windowGetPosition(wnd3, &wnd3PosX, &wnd3PosY);
  windowGetSize(wnd3, &wnd3SizeX, &wnd3SizeY);
  DBGLOG("WND1: Position(%d, %d) Size(%d, %d)\n",
         wnd1PosX, wnd1PosY, wnd1SizeX, wnd1SizeY);
  DBGLOG("WND2: Position(%d, %d) Size(%d, %d)\n",
         wnd2PosX, wnd2PosY, wnd2SizeX, wnd2SizeY);
  DBGLOG("WND3: Position(%d, %d) Size(%d, %d)\n",
         wnd3PosX, wnd3PosY, wnd3SizeX, wnd3SizeY);
  assert(wnd1PosX == 0);
  assert(wnd1PosY == 0);
  assert(wnd1SizeX == WNDMAN_MAX_SPLITS_H);
  assert(wnd1SizeY == WNDMAN_MAX_SPLITS_V / 4
         || wnd1SizeY == WNDMAN_MAX_SPLITS_V / 4 + 1);

  assert(wnd2PosX == 0);
  assert(wnd2PosY == WNDMAN_MAX_SPLITS_V / 2);
  assert(wnd2SizeX == WNDMAN_MAX_SPLITS_H);
  assert(wnd2SizeY == WNDMAN_MAX_SPLITS_V / 2);

  assert(wnd3PosX == 0);
  assert(wnd3PosY == WNDMAN_MAX_SPLITS_V / 4
         || wnd3PosY == WNDMAN_MAX_SPLITS_V / 4 + 1);
  assert(wnd3SizeX == WNDMAN_MAX_SPLITS_H);
  assert(wnd3SizeY == WNDMAN_MAX_SPLITS_V / 4
         || wnd3SizeY == WNDMAN_MAX_SPLITS_V / 4 + 1);

  destroyWindowManager(&wndMan);

  return 0;
}

int windowChainTest(void)
{
  WindowManager wndMan;
  Window *wnd1, *wnd2, *wnd3, *wnd4, *wnd5;
  unsigned wnd1PosX, wnd1PosY, wnd2PosX, wnd2PosY, wnd3PosX, wnd3PosY;
  unsigned wnd1SizeX, wnd1SizeY, wnd2SizeX, wnd2SizeY, wnd3SizeX, wnd3SizeY;

  initWindowManager(&wndMan);
  windowSetBufferName(windowManagerGetWindow(&wndMan), BUF_NAME);

  windowManagerSplitWindow(&wndMan, HORIZONTAL);
  windowManagerSplitWindow(&wndMan, HORIZONTAL);
  windowManagerSplitWindow(&wndMan, HORIZONTAL);

  wnd1 = windowManagerGetWindow(&wndMan);
  windowManagerPrevWindow(&wndMan);
  wnd2 = windowManagerGetWindow(&wndMan);
  assert(wnd1 != wnd2);
  windowManagerPrevWindow(&wndMan);
  wnd3 = windowManagerGetWindow(&wndMan);
  assert(wnd2 != wnd3);
  windowManagerPrevWindow(&wndMan);
  wnd4 = windowManagerGetWindow(&wndMan);
  assert(wnd3 != wnd4);
  windowManagerPrevWindow(&wndMan);
  wnd5 = windowManagerGetWindow(&wndMan);
  assert(wnd5 == wnd1);

  wnd1 = windowManagerGetWindow(&wndMan);
  windowManagerNextWindow(&wndMan);
  wnd2 = windowManagerGetWindow(&wndMan);
  assert(wnd1 != wnd2);
  windowManagerNextWindow(&wndMan);
  wnd3 = windowManagerGetWindow(&wndMan);
  assert(wnd2 != wnd3);
  windowManagerNextWindow(&wndMan);
  wnd4 = windowManagerGetWindow(&wndMan);
  assert(wnd3 != wnd4);
  windowManagerNextWindow(&wndMan);
  wnd5 = windowManagerGetWindow(&wndMan);
  assert(wnd5 == wnd1);

  destroyWindowManager(&wndMan);
  return 0;
}
