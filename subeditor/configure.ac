AC_PREREQ([2.69])
AC_INIT([subeditor], [0.0.1])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([src/buffer.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_PROG_CC
AM_PROG_AR
# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])
LT_INIT

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memmove])

dnl PKG_CHECK_MODULES([LIBSYS], [libsys >= 0.0.1], , [
dnl   AC_MSG_ERROR([library libsys requested from system but not found or not new enough ${prefix}])
dnl ])
dnl AC_SUBST([LIBSYS_CFLAGS])
dnl AC_SUBST([LIBSYS_LIBS])

AC_CONFIG_FILES([Makefile])
AC_SUBST(LIBSUBEDITOR_BASE_CFLAGS, -DDEBUG_MODE)

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
