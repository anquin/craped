libgtkui_la_SOURCES =															\
	src/gtk-ui.c																		\
  src/vte.c																				\
  src/vtefactory.c

noinst_HEADERS =																	\
  include/gtk-ui.h																\
  include/gtkui/vte.h															\
  include/gtkui/vtefactory.h											\
	$(NULL)

noinst_LTLIBRARIES = libgtkui.la

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

libgtkui_la_LIBADD = \
	$(builddir)/../diffex-ui/libdiffexui.la

libgtkui_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(LIBSUBEDITOR_CFLAGS) \
	$(LIBUICORE_CFLAGS) \
	$(DIFFEXUI_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(GTK_CFLAGS) \
	-DGTK_UI \
	$(VTE_CFLAGS) \
	$(NULL)
