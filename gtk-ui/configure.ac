#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gtkui], [0.0.1])
AC_CONFIG_SRCDIR([src/gtk-ui.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CC

PKG_CHECK_MODULES(GTK, gtk+-3.0 >= 1.2.10)
PKG_CHECK_MODULES(VTE, vte-2.90 >= 0.28.2)

LT_INIT

AC_FUNC_MALLOC

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
