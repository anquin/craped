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
	src/fileio.c \
	src/stream.c \
  src/llfile.c \
	src/filesystem.c

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
	include/libsys/stream.h \
	include/libsys/filesystem.h \
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

datatest_SOURCES = test/datatest.c \
	src/print_page_table.c
datatest_CFLAGS = -I$(srcdir)/include
datatest_LDADD = libsys.la

rawdatasearchtest_SOURCES = test/raw_data_search_test.c
rawdatasearchtest_CFLAGS  = -I$(srcdir)/include
rawdatasearchtest_LDADD = libsys.la

rawdatatest_SOURCES = test/raw_data_test_main.c
rawdatatest_CFLAGS  = -I$(srcdir)/include -DTEST_RAW_DATA
rawdatatest_LDADD = libsys.la

pagedrawdatatest_SOURCES = test/raw_data_test_main.c
pagedrawdatatest_CFLAGS  = -I$(srcdir)/include -DTEST_PAGED_RAW_DATA
pagedrawdatatest_LDADD = libsys.la

# TODO: include llfileopentest
check_PROGRAMS = hashtabletest datatest rawdatasearchtest rawdatatest pagedrawdatatest llfileopentest
TESTS = $(check_PROGRAMS)
AM_COLOR_TESTS=always
AM_TESTS_FD_REDIRECT = 9>&2
