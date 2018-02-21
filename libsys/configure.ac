AC_PREREQ([2.69])
AC_INIT([libsys], [0.0.1])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([include/libsys.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
: ${CFLAGS="-g"}
AC_PROG_CC
AM_PROG_AR
LT_INIT

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h stdlib.h string.h])
# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero inet_ntoa memset socket])

AC_CONFIG_FILES([Makefile])

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG_MODE, test x"$debug" = x"true")

AC_OUTPUT
