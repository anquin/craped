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

#include <diffexui/diffex.h>
#include "diffex-fifo.h"

char *de_modes[] = {"erase", "show", "update"};

int ifutil(Diffex *diffex, int b)
{
  int b1;
  switch (diffex->mode) {
  case DE_SHOW:
    deFifoPopPushInt(diffex, NULL, &b); return b;
  case DE_UPDATE:
    deFifoPopPushInt(diffex, &b1, &b);
    if (b == b1) return b;
    diffex->mode = (b ? DE_SHOW : DE_ERASE); return 1;
  case DE_ERASE:
    deFifoPopPushInt(diffex, &b1, NULL); return b1;
  }
}

void initDiffex(Diffex *diffex, unsigned fifoCapacity)
{
  createDEFifo(diffex, fifoCapacity);
}

void destroyDiffex(Diffex *diffex)
{
  destroyDEFifo(diffex);
  free(diffex->fifo);
}
