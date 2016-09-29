libeditor_la_SOURCES = \
	src/editor_impl.c \
	src/editorcmd.c \
	src/editorcmd-funcs.c \
	src/keybinding.c

#include_HEADERS =																\
#	include/editor.h																	\
#	$(NULL)

#lib_LIBRARIES
noinst_LTLIBRARIES = libeditor.la

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

LIBSYS_CFLAGS=-I$(top_srcdir)/../libsys/include
LIBSUBEDITOR_CFLAGS=-I$(top_srcdir)/../subeditor/include
LIBUICORE_CFLAGS=-I$(top_srcdir)/../ui-core/include
LISHARING_CFLAGS=-I$(top_srcdir)/../sharing/include

libeditor_la_LIBADD = \
	$(builddir)/../subeditor/libsubeditor.la \
	$(builddir)/../sharing/libsharing.la \
	$(builddir)/../ui-core/libuicore.la

libeditor_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(LIBSUBEDITOR_CFLAGS) \
	$(LIBUICORE_CFLAGS) \
	$(LISHARING_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)
