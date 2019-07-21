#define FILE_DEFUN(ret_type, func_id, argdecl, arglst) \
  ret_type func_id ## _ll argdecl;                     \
  ret_type func_id ## _stream argdecl;                 \
  ret_type func_id ## _ramFile argdecl;                \
  ret_type func_id argdecl {                           \
    static ret_type (*funcs[]) argdecl =               \
    {                                                  \
      func_id ## _ll,                                  \
      func_id ## _stream                               \
    };                                                 \
    return funcs[fileio->type] arglst;                 \
  }

#include <libsys/fileio.h>

const FileOpenMode FILE_OPEN_MODE_R = 1 << 0;
const FileOpenMode FILE_OPEN_MODE_W = 1 << 1;
/* FILE_OPEN_MODE_R & FILE_OPEN_MODE_W */
const FileOpenMode FILE_OPEN_MODE_RW = 3;
const FileOpenMode FILE_OPEN_MODE_APPEND = 1 << 2;
const FileOpenMode FILE_OPEN_MODE_CREATE = 1 << 3;
const FileOpenMode FILE_OPEN_MODE_TRUNC = 1 << 4;

FileError validateFileOpenMode(FileOpenMode *mode)
{
  if ((*mode & FILE_OPEN_MODE_APPEND) && (*mode & FILE_OPEN_MODE_TRUNC)) {
    return FILE_ERROR_INVALID_OPEN_MODE;
  }

  /* if ((*mode & FILE_OPEN_MODE_W) */
  /*     && !(*mode & FILE_OPEN_MODE_R) */
  /*     && !(*mode & FILE_OPEN_MODE_APPEND)) { */
  /*   *mode |= FILE_OPEN_MODE_TRUNC; */
  /* } */

  return FILE_ERROR_NO_ERROR;
}
