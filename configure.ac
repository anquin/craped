AC_PREREQ([2.69])
AC_INIT([craped], [0.0.1])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CC

AC_ARG_ENABLE(gui,
AS_HELP_STRING([--enable-gui],
[enable gui, default disabled]),
[case "${enableval}" in
yes) enable_gui=true ;;
no)  enable_gui=false ;;
*)   AC_MSG_ERROR([bad value ${enableval} for --enable-gui]) ;;
esac],
[enable_gui=false])
AM_CONDITIONAL(ENABLE_GUI, test x"$enable_gui" = x"true")

# Checks for libraries.
AS_IF([test x"$enable_gui" = x"true"],
  [PKG_CHECK_MODULES(GTK, gtk+-3.0 >= 1.2.10)
   PKG_CHECK_MODULES(VTE, vte-2.90 >= 0.28.2)])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC



AC_CONFIG_SUBDIRS([libsys subeditor ui-core diffex-ui tui sharing editor lib])
if [ x"$enable_gui" = x"true"]; then
  AC_CONFIG_SUBDIRS([gtk-ui])
fi

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
