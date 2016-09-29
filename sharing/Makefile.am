libsharing_la_SOURCES =													\
	src/buffer_server.c														\
	src/sharing_server.c													\
	src/worldcmd.c																\
	src/worldcmd_list_lock.c

noinst_HEADERS = 																		\
	include/sharing.h																	\
	include/sharing/buffer_server.h										\
	include/sharing/sharing_server.h									\
	include/sharing/worldcmd.h												\
	include/sharing/worldcmd_list_lock.h

noinst_LTLIBRARIES = libsharing.la

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

LIBSUBEDITOR_CFLAGS=-I$(top_srcdir)/../subeditor/include
LIBSYS_CFLAGS=-I$(top_srcdir)/../libsys/include

libsharing_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(LIBSYS_CFLAGS) \
	$(LIBSUBEDITOR_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)
