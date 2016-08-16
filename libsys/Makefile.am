libsys_la_SOURCES = \
	src/hashing.c \
	src/quicksearch.c \
	src/socket.c \
	src/page_factory.c \
	src/paging.c \
	src/raw_data.c \
	src/paged_raw_data.c \
	src/search-rawdata.c \
	src/search-paged-rawdata.c \
	src/llfile.c

 # include_HEADERS =	
 # pkginclude_HEADERS =
noinst_HEADERS = \
	include/libsys.h \
	include/libsys/def.h \
	include/libsys/hashing.h \
	include/libsys/socket.h \
	include/libsys/quicksearch.h \
	include/libsys/dbglog.h \
	include/libsys/raw_data.h \
	include/libsys/paged_raw_data.h \
	include/libsys/paging.h \
	include/libsys/fileio.h \
	include/libsys/llfile.h \
	$(NULL)

noinst_LTLIBRARIES = libsys.la

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

libsys_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(BASE_CFLAGS) \
	-I$(srcdir)/include \
	$(NULL)


# TESTS
hashtabletest_SOURCES = test/hashtabletest.c
hashtabletest_CFLAGS = -I$(srcdir)/include
hashtabletest_LDADD = libsys.la
llfileopentest_SOURCES = test/llfileopentest.c
llfileopentest_CFLAGS = -I$(srcdir)/include
llfileopentest_LDADD = libsys.la
check_PROGRAMS = hashtabletest llfileopentest
TESTS = $(check_PROGRAMS)
AM_COLOR_TESTS=always
AM_TESTS_FD_REDIRECT = 9>&2
