SUBDIRS = \
	libsys \
	subeditor \
	sharing \
	ui-core \
	diffex-ui \
	tui \
	editor \
  lib

if ENABLE_GUI
	SUBDIRS += gtk-ui
endif

ACLOCAL_AMFLAGS=-I m4

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

AM_CFLAGS = \
	$(BASE_CFLAGS) \
	-I$(srcdir)/lib/include \
	$(NULL)

# AM_LDFLAGS = -lpthread


# APPLICATION
bin_PROGRAMS=craped
if ENABLE_GUI
	bin_PROGRAMS += gcraped
endif

craped_SOURCES = src/main.c

if ENABLE_GUI
  craped_LDFLAGS = -L$(builddir)/lib/.libs/ -lgcraped
  gcraped_LDFLAGS= $(GTK_LIBS) $(VTE_LIBS)
else
  craped_LDFLAGS = -L$(builddir)/lib/.libs/ -lcraped
endif

# include_HEADERS = include/craped.h
# pkginclude_HEADERS = libsys/include/libsys/def.h
