#include <libsys/filesystem.h>
#include <libsys/llfile.h>

Size fileLength(char *filepath)
{
  Position where;
  LLFile *file;
  FileError error;
  file = fileOpen_ll(filepath, FILE_OPEN_MODE_R, &error);
  fileSeek(FILEIO(file), 0, FILE_IO_WHENCE_END);
  where = fileTell(FILEIO(file));
  fileClose(FILEIO(file));
  return where;
}
