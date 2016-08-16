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
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <libsys/def.h>
#include <raw_data.h>
#include <paged_raw_data.h>
#include <print_page_table.h>

void observerUpdate(struct world_observer *wo, struct world *w) {}

#define MAX_BYTES_SIZE 64

extern Size maxPageSize;

typedef struct test_env {
  Byte s[MAX_BYTES_SIZE];
  Size writeSz;
  Position delPos;
  Size delSz;
} TestEnv;

TestEnv env;

void randStr(void)
{
  Size sz, i;
  static char alphabeta[] = "abcdefghijklmnopqrstuvwxyz";
  unsigned alphabetaSize;

  alphabetaSize = strlen(alphabeta);

  env.writeSz = rand() % MAX_BYTES_SIZE + 1;

  for (i = 0; i < env.writeSz; i++) {
    env.s[i] = alphabeta[rand() % strlen(alphabeta)];
  }
}

void randDel(void)
{
  env.delPos = rand() % (env.writeSz);
  env.delSz = (env.writeSz) - (env.delPos);
}

void fnprint(Byte *what, Size n, FILE *f)
{
  Size i;

  for (i = 0; i < n; i++) {
    fprintf(f, "%c", what[i]);
  }
  fprintf(f, " (%d bytes)\n", n);
}

void printEnv(FILE *out)
{
  fprintf(out, "s=");
  fnprint(env.s, env.writeSz, out);
  fprintf(out, "writeSz=%d\ndelPos=%d\ndelSz=%d\n",
	 env.writeSz, env.delPos, env.delSz);
}

int main(int argc, char *argv[])
{
  int i, len, rawDataNeq;
  RawData data;
  PagedRawData buf;
  Byte *dread, *bread;
  unsigned errCount;
  FILE *envFile;
  unsigned testsMax;
  unsigned testIdx;

  /* Sets the page size to a low value to increase the risks */
  maxPageSize = 16;

  if (argc > 1) {
    testsMax = atoi(argv[1]);
  }
  else {
    testsMax = 128;
  }

  srand(time(0));

  envFile = fopen("lastErrorEnv", "rb");

  if (envFile == NULL) {
    randStr();
    randDel();
  }
  else {
    fread(&env, sizeof(env), 1, envFile);
  }

  for (testIdx = 0; testIdx < testsMax; testIdx++) {
    printf("%d)\n", testIdx + 1);
    envFile = fopen("lastErrorEnv", "wb");
    fwrite(&env, sizeof(env), 1, envFile);
    fclose(envFile);

    rawDataInit(&data, env.writeSz + 3);
    pagedRawDataInit(&buf);

    printEnv(stderr);
    printPageTable(&buf.pageTable, stderr);

    rawDataInsert(&data, 0, env.s, env.writeSz);
    pagedRawDataInsert(&buf, 0, env.s, env.writeSz);

    printPageTable(&buf.pageTable, stderr);

    rawDataDelete(&data, env.delPos, env.delSz);
    pagedRawDataDelete(&buf, env.delPos, env.delSz);

    printPageTable(&buf.pageTable, stderr);

    rawDataInsert(&data, 0, "aaa", 3);
    pagedRawDataInsert(&buf, 0, "aaa", 3);

    printPageTable(&buf.pageTable, stderr);

    dread = (Byte *)malloc(rawDataSize((&data)) * sizeof(Byte));
    bread = (Byte *)malloc(pagedRawDataSize((&buf)) * sizeof(Byte));
    rawDataRead(&data, 0, dread, rawDataSize((&data)));
    pagedRawDataRead(&buf, 0, bread, pagedRawDataSize((&buf)));

    rawDataNeq = memcmp(dread, bread, pagedRawDataSize((&buf)));

    if (rawDataNeq) { 
      printf("ERROR!\n");
      printEnv(stdout);
      printf("\nrawDataCmp=%d\n", rawDataNeq);
      fnprint(env.s, env.writeSz, stdout);
      fnprint(dread, rawDataSize((&data)), stdout);
      fnprint(bread, pagedRawDataSize(&buf), stdout);
      exit(1);
    }
    //printf("OK!\n%s\n%s\n%s\n", env.s, dread, bread);
    randStr();
    randDel();
  }
  printf("%d tests ran 0k\n", testIdx);

  pagedRawDataDestroy(&buf);
  rawDataDestroy(&data);

  return 0;
}
