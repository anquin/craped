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

#ifndef ACTUAL_UI_H
#define ACTUAL_UI_H

#include <ui-core.h>

typedef struct actual_ui ActualUI;

void ActualUIPrepareTerminal(ActualUI *actualUi, DiffexUI *ui,
                             unsigned posX, unsigned posY,
                             unsigned sizeX, unsigned sizeY,
                             Terminal *terminal);
void ActualUIDrawTerminal(ActualUI *actualUi, DiffexUI *ui,
                          Terminal *terminal);
void ActualUIEraseTerminal(ActualUI *actualUi, DiffexUI *ui,
                           Terminal *terminal);

void ActualUIDrawBox(ActualUI *actualUi, DiffexUI *diffexUi,
                     Terminal *terminal);
void ActualUIEraseBox(ActualUI *actualUi, DiffexUI *diffexUi,
                      Terminal *terminal);

unsigned ActualUIGetCenteredTextMax(ActualUI *actualUi, DiffexUI *diffexUi);
void ActualUIDrawCenteredText(ActualUI *actualUi, DiffexUI *diffexUi,
                              char *text, unsigned sz);
void ActualUIEraseCenteredText(ActualUI *actualUi, DiffexUI *diffexUi,
                               char *text, unsigned sz);

void ActualUIDrawStatusLine(ActualUI *actualUi, DiffexUI *ui,
                            unsigned wndPosX, unsigned wndPosY,
                            unsigned wndSizeX, unsigned wndSizeY,
                            char *text, unsigned size);
void ActualUIEraseStatusLine(ActualUI *actualUi, DiffexUI *ui,
                             unsigned wndPosX, unsigned wndPosY,
                             unsigned wndSizeX, unsigned wndSizeY,
                             char *text, unsigned size);

void ActualUIMainLoop(ActualUI *actualUi, UI *ui);

#endif
