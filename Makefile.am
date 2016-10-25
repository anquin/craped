SUBDIRS = \
	libsys \
	subeditor \
	sharing \
	ui-core \
	diffex-ui \
	tui \
	editor \
	gtk-ui

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

AM_CFLAGS = \
	$(BASE_CFLAGS) \
	-I$(srcdir)/libsys/include \
	-I$(srcdir)/editor/include \
	$(NULL)

AM_LDFLAGS = -lpthread


# APPLICATION
bin_PROGRAMS=craped gcraped

craped_SOURCES = src/main.c
craped_LDADD =  $(builddir)/libcraped.la

gcraped_SOURCES = src/main.c
gcraped_LDADD = $(builddir)/libgcraped.la
gcraped_LDFLAGS= $(GTK_LIBS) $(VTE_LIBS)


# LIBRARIES (LIBCRAPED and LIBGCRAPED)
lib_LTLIBRARIES = libcraped.la libgcraped.la
include_HEADERS = src/lib/craped.h
pkginclude_HEADERS = libsys/include/libsys/def.h

CRAPED_LIBS_SOURCES = \
	src/lib/setup.c \
	src/lib/craped.c \
	src/lib/craped-editor-subscriber.c \
	$(NULL)

CRAPED_LIBS_CFLAGS = \
	-I$(srcdir)/libsys/include \
	-I$(srcdir)/subeditor/include \
	-I$(srcdir)/sharing/include \
	-I$(srcdir)/ui-core/include \
	-I$(srcdir)/diffex-ui/include \
	-I$(srcdir)/editor/include \
	$(NULL)

libcraped_la_SOURCES = \
	$(CRAPED_LIBS_SOURCES) \
	src/lib/setup-tui.c \
	$(NULL)
libcraped_la_CFLAGS = \
	$(CRAPED_LIBS_CFLAGS) \
	-I$(srcdir)/tui/include \
	$(NULL)
libcraped_la_LIBADD = \
	$(builddir)/editor/libeditor.la \
	$(builddir)/tui/libtui.la \
	$(NULL)

libgcraped_la_SOURCES = \
	$(CRAPED_LIBS_SOURCES) \
	src/lib/setup-gtk-ui.c \
	$(NULL)
libgcraped_la_CFLAGS = \
	$(CRAPED_LIBS_CFLAGS) \
	-I$(srcdir)/gtk-ui/include \
	$(GTK_CFLAGS) \
	$(NULL)
libgcraped_la_LIBADD = \
	$(builddir)/editor/libeditor.la \
	$(builddir)/gtk-ui/libgtkui.la \
	$(NULL)
libgcraped_la_LDFLAGS = $(GTK_LIBS) $(VTE_LIBS)
