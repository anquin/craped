libuicore_la_SOURCES =													\
	src/buffer_point_moves.c											\
	src/framer.c																	\
	src/input.c																		\
	src/keys.c																		\
	src/statusline.c															\
	src/ui.c																			\
	src/window.c																	\
	src/windowmanager.c

noinst_HEADERS =  													\
	include/ui-core.h    											\
	include/uicore/box.h											\
	include/uicore/buffer_point_moves.h				\
	include/uicore/framer.h										\
	include/uicore/input.h										\
	include/uicore/keys.h											\
	include/uicore/statusline.h								\
	include/uicore/terminal.h									\
	include/uicore/terminalfactory.h					\
	include/uicore/ui-private.h								\
	include/uicore/ui.h												\
	include/uicore/ui_observer.h							\
	include/uicore/window.h										\
	include/uicore/window-reference.h					\
	$(NULL)

noinst_LTLIBRARIES = libuicore.la

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

LIBSYS_CFLAGS=-I$(top_srcdir)/../libsys/include
LIBSUBEDITOR_CFLAGS=-I$(top_srcdir)/../subeditor/include

libuicore_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(LIBSUBEDITOR_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)


# TESTS
check_PROGRAMS = split_wnd_test windowidtest fetch-lines
TESTS = $(check_PROGRAMS)
AM_COLOR_TESTS=always
AM_TESTS_FD_REDIRECT = 9>&2

UICORE_LIBSDEPS = $(builddir)/../libsys/libsys.la $(builddir)/../subeditor/libsubeditor.la
UICORE_BASE_CFLAGS = -I$(srcdir)/include -I$(srcdir)/src $(LIBSYS_CFLAGS) $(LIBSUBEDITOR_CFLAGS)

split_wnd_test_SOURCES = test/split_wnd_test.c \
	$(libuicore_la_SOURCES)

windowidtest_SOURCES = test/windowidtest.c \
	$(libuicore_la_SOURCES)

fetch_lines_SOURCES = test/fetch_lines.c \
	$(libuicore_la_SOURCES)

split_wnd_test_CFLAGS = $(UICORE_BASE_CFLAGS)
split_wnd_test_LDADD = $(UICORE_LIBSDEPS)
windowidtest_CFLAGS = $(UICORE_BASE_CFLAGS)
windowidtest_LDADD = $(UICORE_LIBSDEPS)
fetch_lines_CFLAGS = $(UICORE_BASE_CFLAGS)
fetch_lines_LDADD = $(UICORE_LIBSDEPS)
