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
