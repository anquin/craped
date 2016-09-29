bin_PROGRAMS=craped gcraped

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

craped_SOURCES = src/main.c
craped_LDADD =  $(builddir)/libcraped.la

gcraped_SOURCES = src/main.c
gcraped_LDADD = $(builddir)/libgcraped.la
gcraped_LDFLAGS= $(GTK_LIBS) $(VTE_LIBS)


# LIBCRAPED and LIBGCRAPED

include_HEADERS = src/lib/craped.h
pkginclude_HEADERS = src/lib/craped.h
noinst_LTLIBRARIES = libcraped.la libgcraped.la

libcraped_la_SOURCES = \
	src/lib/setup.c \
	src/lib/setup-tui.c \
	src/lib/craped.c \
	$(NULL)

libcraped_la_LIBADD = \
	$(builddir)/editor/libeditor.la \
	$(builddir)/tui/libtui.la \
	$(NULL)

libcraped_la_CFLAGS = \
	-I$(srcdir)/libsys/include \
	-I$(srcdir)/subeditor/include \
	-I$(srcdir)/ui-core/include \
	-I$(srcdir)/diffex-ui/include \
	-I$(srcdir)/tui/include \
	-I$(srcdir)/sharing/include \
	-I$(srcdir)/editor/include \
	$(NULL)

libgcraped_la_SOURCES = \
	src/lib/setup.c \
	src/lib/setup-gtk-ui.c \
	src/lib/craped.c \
	$(NULL)

libgcraped_la_LIBADD = \
	$(builddir)/editor/libeditor.la \
	$(builddir)/gtk-ui/libgtkui.la \
	$(NULL)

libgcraped_la_CFLAGS = \
	-I$(srcdir)/libsys/include \
	-I$(srcdir)/subeditor/include \
	-I$(srcdir)/ui-core/include \
	-I$(srcdir)/diffex-ui/include \
	-I$(srcdir)/sharing/include \
	-I$(srcdir)/editor/include \
	-I$(srcdir)/gtk-ui/include \
	-DGTK_UI \
	$(GTK_CFLAGS) \
	$(VTE_CFLAGS) \
	$(AM_CFLAGS) \
	$(CFLAGS_GTK) \
	$(NULL)

libgcraped_la_LDFLAGS = $(GTK_LIBS) $(VTE_LIBS)
