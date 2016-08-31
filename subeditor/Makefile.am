libsubeditor_la_SOURCES = \
	src/buffer.c \
	src/buffer_commands.c \
	src/mark.c \
	src/mode.c \
	src/subeditor.c

 # include_HEADERS =
 # pkginclude_HEADERS =
noinst_HEADERS = 													\
	include/subeditor.h											\
	include/subeditor/def.h 	   	          \
	include/subeditor/world_observer.h	  	\
	include/subeditor/mode.h  	         	  \
	$(NULL)

noinst_LTLIBRARIES = libsubeditor.la

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

LIBSYS_CFLAGS=-I$(top_srcdir)/../libsys/include

libsubeditor_la_LIBADD=$(builddir)/../libsys/libsys.la

libsubeditor_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)


# TESTS
check_PROGRAMS = bufcmd
TESTS = $(check_PROGRAMS)
AM_COLOR_TESTS=always
AM_TESTS_FD_REDIRECT = 9>&2

SUBEDITOR_BASE_CFLAGS = -I$(srcdir)/include -I$(srcdir)/src $(LIBSYS_CFLAGS)
SUBEDITOR_LIBSDEPS = $(builddir)/../libsys/libsys.la

bufcmd_SOURCES = test/bufcmd.c \
	$(libsubeditor_la_SOURCES)

bufcmd_CFLAGS = $(SUBEDITOR_BASE_CFLAGS)
bufcmd_LDADD = $(SUBEDITOR_LIBSDEPS)
