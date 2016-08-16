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

#include <uicore/statusline.h>
#include <stdlib.h>

static char *blankLine = "";

struct status_line
{
  unsigned len;
  char *text;
};

StatusLine *createStatusLine(void)
{
  StatusLine *sl;
  sl = (StatusLine *)malloc(sizeof(StatusLine));
  initStatusLine(sl);
}

void initStatusLine(StatusLine *statusLine)
{
  statusLine->text = blankLine;
}

void destroyStatusLine(StatusLine *statusLine)
{
  if (statusLine->text != blankLine) {
    free(statusLine->text);
  }
}

void statusLineSetText(StatusLine *statusLine, char *text, unsigned len)
{
  unsigned i;
  if (statusLine->text == blankLine) {
    statusLine->text = (char *)malloc(sizeof(char) * len + 1);
  } else if (len > statusLine->len) {
    statusLine->text = realloc(statusLine->text, len + 1);
  }
  for (i = 0; i < len; i++) {
    statusLine->text[i] = text[i];
  }
  statusLine->text[i] = '\0';
}

char *statusLineGetText(StatusLine *statusLine)
{
  return statusLine->text;
}
