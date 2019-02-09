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

#include <stdlib.h>

#include <editor.h>
#include "craped-editor-subscriber.h"

struct craped_editor_subscriber
{
  CrapedSubscriber *impl;
  Craped *craped;
};

CrapedSubscriber *createCrapedSubscriber(void *impl, CrapedSubscriberFn fn)
{
  CrapedSubscriber *subscriber;

  subscriber = (CrapedSubscriber *)malloc(sizeof(CrapedSubscriber));
  subscriber->impl = impl;
  subscriber->fn = fn;

  return subscriber;
}

void crapedEditorSubscriberFn(void *subscriber, Editor *editor)
{
  ((CrapedEditorSubscriber *)subscriber)->impl->fn
    (((CrapedEditorSubscriber *)subscriber)->impl->impl,
     ((CrapedEditorSubscriber *)subscriber)->craped);
}

CrapedEditorSubscriber *createCrapedEditorSubscriber(void *impl, Craped *craped) {
  CrapedEditorSubscriber *crapedEditorSubscriber;
  crapedEditorSubscriber =
    (CrapedEditorSubscriber *)malloc(sizeof(CrapedEditorSubscriber));
  crapedEditorSubscriber->impl = impl;
  crapedEditorSubscriber->craped = craped;
  return crapedEditorSubscriber;
}
