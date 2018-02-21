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

#ifndef CRAPED_EDITOR_SUBSCRIBER_H
#define CRAPED_EDITOR_SUBSCRIBER_H

#include "craped.h"

typedef struct craped_editor_subscriber CrapedEditorSubscriber;
void crapedEditorSubscriberFn(void *subscriber, Editor *editor);
CrapedEditorSubscriber *createCrapedEditorSubscriber(void *impl, Craped *craped);

#endif
