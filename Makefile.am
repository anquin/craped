bin_PROGRAMS=craped gcraped

SUBDIRS = \
	libsys \
	subeditor \
	sharing \
	ui-core \
	diffex-ui \
	tui \
	editor \
	gtk-ui

X_BASE_CFLAGS= # -Wall
if DEBUG_MODE
BASE_CFLAGS=$(X_BASE_CFLAGS) -DDEBUG_MODE
else
BASE_CFLAGS=$(X_BASE_CFLAGS) -O2 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations
endif

AM_CFLAGS = \
	$(BASE_CFLAGS) \
	-I$(srcdir)/libsys/include \
	-I$(srcdir)/subeditor/include \
	-I$(srcdir)/ui-core/include \
	-I$(srcdir)/diffex-ui/include \
	-I$(srcdir)/tui/include \
	-I$(srcdir)/sharing/include \
	-I$(srcdir)/editor/include \
	-I$(srcdir)/gtk-ui/include

AM_LDFLAGS = -lpthread

craped_SOURCES = \
	src/main.c \
	src/setup.c \
	src/setup-tui.c \
	$(NULL)

craped_LDADD = \
	$(builddir)/editor/libeditor.la \
	$(builddir)/tui/libtui.la

gcraped_SOURCES = \
	src/main.c \
	src/setup.c \
	src/setup-gtk-ui.c \
	$(NULL)

gcraped_CFLAGS = \
	-DGTK_UI \
	$(GTK_CFLAGS) \
	$(VTE_CFLAGS) \
	$(AM_CFLAGS) \
	$(CFLAGS_GTK) \
	$(NULL)

gcraped_LDADD = \
	$(builddir)/editor/libeditor.la \
	$(builddir)/gtk-ui/libgtkui.la

gcraped_LDFLAGS= $(GTK_LIBS) $(VTE_LIBS)
