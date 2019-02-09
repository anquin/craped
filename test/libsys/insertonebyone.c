#include <assert.h>
#include <libsys/paged_raw_data.h>
#include <dbg/print_page_table.h>

int main(void)
{
  Byte *chunk;
  PagedRawData data;
  pagedRawDataInit(&data);
  pagedRawDataInsert(&data, 0, "A", 1);
  pagedRawDataInsert(&data, 1, "B", 1);
  pagedRawDataInsert(&data, 2, "C", 1);
  pagedRawDataInsert(&data, 3, "D", 1);
  printf("pagedRawDataSize(&data): %d\n\n", pagedRawDataSize(&data));
  assert(pagedRawDataSize(&data) == 4);
  assert(memcmp("ABCD", chunk, pagedRawDataSize(&data)));
  printPageTable(&data.pageTable, stdout);

  printf("========================================\n\n");

  pagedRawDataDelete(&data, 0, pagedRawDataSize(&data));
  printf("pagedRawDataSize(&data): %d\n\n", pagedRawDataSize(&data));
  assert(pagedRawDataSize(&data) == 0);
  printPageTable(&data.pageTable, stdout);

  pagedRawDataDestroy(&data);
  return 0;
}
