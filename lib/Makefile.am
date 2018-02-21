ACLOCAL_AMFLAGS = -I m4

CRAPED_LIBS_SRCS =               \
	src/craped-editor-subscriber.c    \
	src/craped.c                      \
	src/setup.c                       \
$(NULL)

GCRAPED_LIBS_SOURCES = $(CRAPED_LIBS_SRCS) src/setup-gtk-ui.c
TCRAPED_LIBS_SOURCES = $(CRAPED_LIBS_SRCS) src/setup-tui.c

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
-fomit-frame-pointer -fexpensive-optimizations
endif

AM_CFLAGS = \
$(BASE_CFLAGS) \
-I$(top_srcdir)../libsys/include \
-I$(top_srcdir)../editor/include \
$(NULL)

AM_LDFLAGS = -lpthread

include_HEADERS = \
	include/craped.h \
	$(NULL)
pkginclude_HEADERS = $(top_srcdir)/../libsys/include/libsys/def.h

lib_LTLIBRARIES = libcraped.la
if ENABLE_GUI
lib_LTLIBRARIES += libgcraped.la
endif


CRAPED_LIBS_CFLAGS = \
-I$(top_srcdir)/../libsys/include \
-I$(top_srcdir)/../subeditor/include \
-I$(top_srcdir)/../sharing/include \
-I$(top_srcdir)/../ui-core/include \
-I$(top_srcdir)/../diffex-ui/include \
-I$(top_srcdir)/../editor/include \
-I$(srcdir)/include \
$(NULL)

CRAPED_LIBS = \
$(builddir)/../editor/libeditor.la \
$(NULL)

GCRAPED_LIBS = $(CRAPED_LIBS) $(builddir)/../tui/libgtkui.la
GCRAPED_LIBS_CFLAGS = $(CRAPED_LIBS_CFLAGS) -I$(top_srcdir)/../gtk-ui/include
TCRAPED_LIBS = $(CRAPED_LIBS) $(builddir)/../tui/libtui.la
TCRAPED_LIBS_CFLAGS = $(CRAPED_LIBS_CFLAGS) -I$(top_srcdir)/../tui/include

libcraped_la_SOURCES = $(TCRAPED_LIBS_SOURCES)
libcraped_la_CFLAGS = $(BASE_CFLAGS) $(TCRAPED_LIBS_CFLAGS)
libcraped_la_LIBADD = $(TCRAPED_LIBS)

if ENABLE_GUI
libgcraped_la_SOURCES = $(GCRAPED_LIBS_SOURCES)
libgcraped_la_CFLAGS = $(BASE_CFLAGS) $(GCRAPED_LIBS_CFLAGS)
libgcraped_la_LIBADD = $(GCRAPED_LIBS)
endif
