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

#include <uicore/window.h>
#include <uicore/windowmanager.h>

#include <libsys/dbglog.h>
#include <assert.h>

void observerUpdate(struct world_observer *wo, struct world *w) {}
unsigned terminalGetHeight(struct terminal *t) {}

/* #ifdef WNDMAN_MAX_SPLITS_V */
/* #undef WNDMAN_MAX_SPLITS_V */
/* #define WNDMAN_MAX_SPLITS_V 20 */
/* #endif */

int main(void)
{
  unsigned wid;
  WindowManager wndMan;

  /* DBGLOG_OPEN(stdout); */

  initWindowManager(&wndMan);
  windowSetBufferName(windowManagerGetWindow(&wndMan), "lala");

  wid = windowGetId(windowManagerGetWindow(&wndMan));
  assert(wid == 1);
  wid = windowManagerSplitWindow(&wndMan, HORIZONTAL);
  assert(wid == 2);
  wid = windowManagerSplitWindow(&wndMan, HORIZONTAL);
  assert(wid == 3);
  windowManagerRemoveCurrentWindow(&wndMan);
  wid = windowManagerSplitWindow(&wndMan, HORIZONTAL);
  assert(wid == 1);
  wid = windowManagerSplitWindow(&wndMan, HORIZONTAL);
  assert(wid == 4);

  destroyWindowManager(&wndMan);

  DBGLOG_CLOSE();

  return 0;
}
