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

#include "frontend.h"
#include "editor_impl.h"

#include <stdlib.h>

struct editor_subscriber
{
  EditorSubscriberFn fn;
  void *impl;
};

EditorSubscriber *createEditorSubscriber(void *impl, EditorSubscriberFn fn)
{
  EditorSubscriber *subscriber =
    (EditorSubscriber *)malloc(sizeof(EditorSubscriber));
  subscriber->impl = impl;
  subscriber->fn = fn;
  return subscriber;
}
void destroyEditorSubscriber(EditorSubscriber *subscriber) { }

typedef struct editor_subscriber_chain_node
{
  EditorSubscriber *subscriber;
  struct editor_subscriber_chain_node *next;
} EditorSubscriberChainNode;

struct world_observer
{
  Editor *editor;
  UI *ui;
  EditorSubscriberChainNode *subscriberChain;
  short isHandling;
};

EditorFrontend *createEditorFrontend(UI *ui)
{
  EditorFrontend *fe;
  fe = (EditorFrontend *)malloc(sizeof(EditorFrontend));
  fe->isHandling = 0;
  fe->subscriberChain = NULL;
  fe->ui = ui;
  return fe;
}

void destroyEditorFrontend(EditorFrontend *frontend)
{
  EditorSubscriberChainNode *tmp;
  while (frontend->subscriberChain != NULL) {
    tmp = frontend->subscriberChain;
    frontend->subscriberChain = frontend->subscriberChain->next;
    free(tmp);
  }
  frontend->isHandling = 1;  /* Better prevent something from happening */
}

void editorFrontendAddSubscriber(EditorFrontend *frontend,
                                 EditorSubscriber *subscriber)
{
  EditorSubscriberChainNode *curr, *newNode;;
  newNode =
    (EditorSubscriberChainNode *)malloc(sizeof(EditorSubscriberChainNode));
  newNode->subscriber = subscriber;
  newNode->next = NULL;
  if (frontend->subscriberChain == NULL) {
    frontend->subscriberChain = newNode;
  }
  else {
    curr = frontend->subscriberChain;
    while (curr->next != NULL) {
      curr = curr->next;
    }
    curr->next = newNode;
  }
}

void observerUpdate(EditorFrontend *frontend, World *world)
{
  EditorSubscriberChainNode *subsNode;
  if (!worldIsAlive(world)) {
    return;
  }
  if (!frontend->isHandling) { /* prevent subscribers from generating further */
    frontend->isHandling = 1;  /* update calls, avoiding infinite recursion   */
    subsNode = frontend->subscriberChain;
    while (subsNode != NULL) {
      subsNode->subscriber->fn(subsNode->subscriber->impl, frontend->editor);
      subsNode = subsNode->next;
    }
    uiRedisplay(frontend->ui, world);
    frontend->isHandling = 0;
  }
}
