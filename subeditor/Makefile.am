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
check_PROGRAMS = bufcmd datatest rawdatasearchtest rawdatatest pagedrawdatatest
TESTS = $(check_PROGRAMS)
AM_COLOR_TESTS=always
AM_TESTS_FD_REDIRECT = 9>&2

SUBEDITOR_BASE_CFLAGS = -I$(srcdir)/include -I$(srcdir)/src $(LIBSYS_CFLAGS)
SUBEDITOR_LIBSDEPS = $(builddir)/../libsys/libsys.la

bufcmd_SOURCES = test/bufcmd.c \
	$(libsubeditor_la_SOURCES)

datatest_SOURCES = test/datatest.c \
	src/print_page_table.c \
	$(libsubeditor_la_SOURCES)

rawdatasearchtest_SOURCES = test/raw_data_search_test.c \
	$(libsubeditor_la_SOURCES)

rawdatatest_SOURCES = test/raw_data_test_main.c \
	$(libsubeditor_la_SOURCES)

pagedrawdatatest_SOURCES = test/raw_data_test_main.c \
	$(libsubeditor_la_SOURCES)

datatest_CFLAGS = $(SUBEDITOR_BASE_CFLAGS)
datatest_LDADD = $(SUBEDITOR_LIBSDEPS)
bufcmd_CFLAGS = $(SUBEDITOR_BASE_CFLAGS)
bufcmd_LDADD = $(SUBEDITOR_LIBSDEPS)
rawdatasearchtest_CFLAGS = $(SUBEDITOR_BASE_CFLAGS)
rawdatasearchtest_LDADD = $(SUBEDITOR_LIBSDEPS)
rawdatatest_CFLAGS = -DTEST_RAW_DATA $(SUBEDITOR_BASE_CFLAGS)
rawdatatest_LDADD = $(SUBEDITOR_LIBSDEPS)
pagedrawdatatest_CFLAGS = -DTEST_PAGED_RAW_DATA $(SUBEDITOR_BASE_CFLAGS)
pagedrawdatatest_LDADD = $(SUBEDITOR_LIBSDEPS)
