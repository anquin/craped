ACLOCAL_AMFLAGS = -I m4

libdiffexui_la_SOURCES =													\
	src/diffex-ui-update.c												\
	src/diffex-ui.c																\
	src/redisplay-utils.c													\
	src/diffex-ui-private.c												\
	src/diffex.c																	\
	src/diffex-fifo.c

noinst_HEADERS =																\
	include/diffex-ui.h         									\
	include/diffexui/diffex-ui-private.h					\
	include/diffexui/diffex.h	 										\
	include/diffexui/actual-ui.h									\
	$(NULL)

noinst_LTLIBRARIES = libdiffexui.la

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

libdiffexui_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(LIBSUBEDITOR_CFLAGS) \
	$(LIBUICORE_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)
