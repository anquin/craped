ACLOCAL_AMFLAGS = -I m4

libtui_la_SOURCES =															\
	src/tui.c																			\
  src/xterm.c																		\
  src/xtermfactory.c

noinst_HEADERS =																\
  include/tui.h																	\
  include/tui/xterm.h														\
  include/tui/xtermfactory.h										\
	$(NULL)

noinst_LTLIBRARIES = libtui.la

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
DIFFEXUI_CFLAGS=-I$(top_srcdir)/../diffex-ui/include

libtui_la_LIBADD = \
	$(builddir)/../diffex-ui/libdiffexui.la

libtui_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(LIBSUBEDITOR_CFLAGS) \
	$(LIBUICORE_CFLAGS) \
	$(DIFFEXUI_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)
