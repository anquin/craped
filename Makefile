CC=gcc
CFLAGS=-g -c -I./include/ -DSUBEDITOR_INLINE_= -DPACKAGE_STRING=\"craped\"
LDFLAGS=-lpthread

.PHONY: all clean craped dirs

all: craped

clean:
	find ./build/obj/ ! -type d -exec rm -rf '{}' \;

dirs:
	mkdir -p ./build/obj/diffex-ui
	mkdir -p ./build/obj/editor
	mkdir -p ./build/obj/lib
	mkdir -p ./build/obj/libsys
	mkdir -p ./build/obj/sharing
	mkdir -p ./build/obj/subeditor
	mkdir -p ./build/obj/tui
	mkdir -p ./build/obj/ui-core

craped: dirs ./build/craped

./build/obj/diffex-ui/diffex-fifo.o: ./src/diffex-ui/diffex-fifo.c
	$(CC) $(CFLAGS) -o ./build/obj/diffex-ui/diffex-fifo.o ./src/diffex-ui/diffex-fifo.c
./build/obj/diffex-ui/diffex-ui-private.o: ./src/diffex-ui/diffex-ui-private.c
	$(CC) $(CFLAGS) -o ./build/obj/diffex-ui/diffex-ui-private.o ./src/diffex-ui/diffex-ui-private.c
./build/obj/diffex-ui/diffex-ui-update.o: ./src/diffex-ui/diffex-ui-update.c
	$(CC) $(CFLAGS) -o ./build/obj/diffex-ui/diffex-ui-update.o ./src/diffex-ui/diffex-ui-update.c
./build/obj/diffex-ui/diffex-ui.o: ./src/diffex-ui/diffex-ui.c
	$(CC) $(CFLAGS) -o ./build/obj/diffex-ui/diffex-ui.o ./src/diffex-ui/diffex-ui.c
./build/obj/diffex-ui/diffex.o: ./src/diffex-ui/diffex.c
	$(CC) $(CFLAGS) -o ./build/obj/diffex-ui/diffex.o ./src/diffex-ui/diffex.c
./build/obj/diffex-ui/redisplay-utils.o: ./src/diffex-ui/redisplay-utils.c
	$(CC) $(CFLAGS) -o ./build/obj/diffex-ui/redisplay-utils.o ./src/diffex-ui/redisplay-utils.c
./build/obj/editor/editor_impl.o: ./src/editor/editor_impl.c
	$(CC) $(CFLAGS) -o ./build/obj/editor/editor_impl.o ./src/editor/editor_impl.c
./build/obj/editor/editorcmd-funcs.o: ./src/editor/editorcmd-funcs.c
	$(CC) $(CFLAGS) -o ./build/obj/editor/editorcmd-funcs.o ./src/editor/editorcmd-funcs.c
./build/obj/editor/editorcmd.o: ./src/editor/editorcmd.c
	$(CC) $(CFLAGS) -o ./build/obj/editor/editorcmd.o ./src/editor/editorcmd.c
./build/obj/editor/frontend.o: ./src/editor/frontend.c
	$(CC) $(CFLAGS) -o ./build/obj/editor/frontend.o ./src/editor/frontend.c
./build/obj/editor/keybinding.o: ./src/editor/keybinding.c
	$(CC) $(CFLAGS) -o ./build/obj/editor/keybinding.o ./src/editor/keybinding.c
./build/obj/lib/craped-editor-subscriber.o: ./src/lib/craped-editor-subscriber.c
	$(CC) $(CFLAGS) -o ./build/obj/lib/craped-editor-subscriber.o ./src/lib/craped-editor-subscriber.c
./build/obj/lib/craped.o: ./src/lib/craped.c
	$(CC) $(CFLAGS) -o ./build/obj/lib/craped.o ./src/lib/craped.c
./build/obj/lib/setup-tui.o: ./src/lib/setup-tui.c
	$(CC) $(CFLAGS) -o ./build/obj/lib/setup-tui.o ./src/lib/setup-tui.c
./build/obj/lib/setup.o: ./src/lib/setup.c
	$(CC) $(CFLAGS) -o ./build/obj/lib/setup.o ./src/lib/setup.c
./build/obj/libsys/fileio.o: ./src/libsys/fileio.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/fileio.o ./src/libsys/fileio.c
./build/obj/libsys/filesystem.o: ./src/libsys/filesystem.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/filesystem.o ./src/libsys/filesystem.c
./build/obj/libsys/hashing.o: ./src/libsys/hashing.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/hashing.o ./src/libsys/hashing.c
./build/obj/libsys/llfile.o: ./src/libsys/llfile.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/llfile.o ./src/libsys/llfile.c
./build/obj/libsys/page_factory.o: ./src/libsys/page_factory.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/page_factory.o ./src/libsys/page_factory.c
./build/obj/libsys/mem.o: ./src/libsys/mem.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/mem.o ./src/libsys/mem.c
./build/obj/libsys/paged_raw_data.o: ./src/libsys/paged_raw_data.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/paged_raw_data.o ./src/libsys/paged_raw_data.c
./build/obj/libsys/paging.o: ./src/libsys/paging.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/paging.o ./src/libsys/paging.c
./build/obj/libsys/print_page_table.o: ./src/libsys/print_page_table.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/print_page_table.o ./src/libsys/print_page_table.c
./build/obj/libsys/quicksearch.o: ./src/libsys/quicksearch.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/quicksearch.o ./src/libsys/quicksearch.c
./build/obj/libsys/raw_data.o: ./src/libsys/raw_data.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/raw_data.o ./src/libsys/raw_data.c
./build/obj/libsys/search-paged-rawdata.o: ./src/libsys/search-paged-rawdata.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/search-paged-rawdata.o ./src/libsys/search-paged-rawdata.c
./build/obj/libsys/search-rawdata.o: ./src/libsys/search-rawdata.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/search-rawdata.o ./src/libsys/search-rawdata.c
./build/obj/libsys/socket.o: ./src/libsys/socket.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/socket.o ./src/libsys/socket.c
./build/obj/libsys/stream.o: ./src/libsys/stream.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/stream.o ./src/libsys/stream.c
./build/obj/libsys/strhash.o: ./src/libsys/strhash.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/strhash.o ./src/libsys/strhash.c
./build/obj/libsys/strutil.o: ./src/libsys/strutil.c
	$(CC) $(CFLAGS) -o ./build/obj/libsys/strutil.o ./src/libsys/strutil.c
./build/obj/sharing/buffer_server.o: ./src/sharing/buffer_server.c
	$(CC) $(CFLAGS) -o ./build/obj/sharing/buffer_server.o ./src/sharing/buffer_server.c
./build/obj/sharing/sharing_server.o: ./src/sharing/sharing_server.c
	$(CC) $(CFLAGS) -o ./build/obj/sharing/sharing_server.o ./src/sharing/sharing_server.c
./build/obj/sharing/worldcmd.o: ./src/sharing/worldcmd.c
	$(CC) $(CFLAGS) -o ./build/obj/sharing/worldcmd.o ./src/sharing/worldcmd.c
./build/obj/sharing/worldcmd_list_lock.o: ./src/sharing/worldcmd_list_lock.c
	$(CC) $(CFLAGS) -o ./build/obj/sharing/worldcmd_list_lock.o ./src/sharing/worldcmd_list_lock.c
./build/obj/subeditor/buffer.o: ./src/subeditor/buffer.c
	$(CC) $(CFLAGS) -o ./build/obj/subeditor/buffer.o ./src/subeditor/buffer.c
./build/obj/subeditor/buffer_commands.o: ./src/subeditor/buffer_commands.c
	$(CC) $(CFLAGS) -o ./build/obj/subeditor/buffer_commands.o ./src/subeditor/buffer_commands.c
./build/obj/subeditor/mark.o: ./src/subeditor/mark.c
	$(CC) $(CFLAGS) -o ./build/obj/subeditor/mark.o ./src/subeditor/mark.c
./build/obj/subeditor/mode.o: ./src/subeditor/mode.c
	$(CC) $(CFLAGS) -o ./build/obj/subeditor/mode.o ./src/subeditor/mode.c
./build/obj/subeditor/subeditor.o: ./src/subeditor/subeditor.c
	$(CC) $(CFLAGS) -o ./build/obj/subeditor/subeditor.o ./src/subeditor/subeditor.c
./build/obj/tui/tui.o: ./src/tui/tui.c
	$(CC) $(CFLAGS) -o ./build/obj/tui/tui.o ./src/tui/tui.c
./build/obj/tui/xterm.o: ./src/tui/xterm.c
	$(CC) $(CFLAGS) -o ./build/obj/tui/xterm.o ./src/tui/xterm.c
./build/obj/tui/xtermfactory.o: ./src/tui/xtermfactory.c
	$(CC) $(CFLAGS) -o ./build/obj/tui/xtermfactory.o ./src/tui/xtermfactory.c
./build/obj/ui-core/buffer_point_moves.o: ./src/ui-core/buffer_point_moves.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/buffer_point_moves.o ./src/ui-core/buffer_point_moves.c
./build/obj/ui-core/framer.o: ./src/ui-core/framer.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/framer.o ./src/ui-core/framer.c
./build/obj/ui-core/input.o: ./src/ui-core/input.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/input.o ./src/ui-core/input.c
./build/obj/ui-core/keys.o: ./src/ui-core/keys.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/keys.o ./src/ui-core/keys.c
./build/obj/ui-core/statusline.o: ./src/ui-core/statusline.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/statusline.o ./src/ui-core/statusline.c
./build/obj/ui-core/ui.o: ./src/ui-core/ui.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/ui.o ./src/ui-core/ui.c
./build/obj/ui-core/window.o: ./src/ui-core/window.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/window.o ./src/ui-core/window.c
./build/obj/ui-core/windowmanager.o: ./src/ui-core/windowmanager.c
	$(CC) $(CFLAGS) -o ./build/obj/ui-core/windowmanager.o ./src/ui-core/windowmanager.c
./build/obj/main.o: ./src/main.c
	$(CC) $(CFLAGS) -o ./build/obj/main.o ./src/main.c

OBJS= ./build/obj/diffex-ui/diffex-fifo.o ./build/obj/diffex-ui/diffex-ui-private.o ./build/obj/diffex-ui/diffex-ui-update.o ./build/obj/diffex-ui/diffex-ui.o ./build/obj/diffex-ui/diffex.o ./build/obj/diffex-ui/redisplay-utils.o ./build/obj/editor/editor_impl.o ./build/obj/editor/editorcmd-funcs.o ./build/obj/editor/editorcmd.o ./build/obj/editor/frontend.o ./build/obj/editor/keybinding.o ./build/obj/lib/craped-editor-subscriber.o ./build/obj/lib/craped.o ./build/obj/lib/setup-tui.o ./build/obj/lib/setup.o ./build/obj/libsys/fileio.o ./build/obj/libsys/filesystem.o ./build/obj/libsys/hashing.o ./build/obj/libsys/llfile.o ./build/obj/libsys/mem.o ./build/obj/libsys/page_factory.o ./build/obj/libsys/paged_raw_data.o ./build/obj/libsys/paging.o ./build/obj/libsys/print_page_table.o ./build/obj/libsys/quicksearch.o ./build/obj/libsys/raw_data.o ./build/obj/libsys/search-paged-rawdata.o ./build/obj/libsys/search-rawdata.o ./build/obj/libsys/socket.o ./build/obj/libsys/stream.o ./build/obj/libsys/strhash.o ./build/obj/libsys/strutil.o ./build/obj/sharing/buffer_server.o ./build/obj/sharing/sharing_server.o ./build/obj/sharing/worldcmd.o ./build/obj/sharing/worldcmd_list_lock.o ./build/obj/subeditor/buffer.o ./build/obj/subeditor/buffer_commands.o ./build/obj/subeditor/mark.o ./build/obj/subeditor/mode.o ./build/obj/subeditor/subeditor.o ./build/obj/tui/tui.o ./build/obj/tui/xterm.o ./build/obj/tui/xtermfactory.o ./build/obj/ui-core/buffer_point_moves.o ./build/obj/ui-core/framer.o ./build/obj/ui-core/input.o ./build/obj/ui-core/keys.o ./build/obj/ui-core/statusline.o ./build/obj/ui-core/ui.o ./build/obj/ui-core/window.o ./build/obj/ui-core/windowmanager.o ./build/obj/main.o

./build/craped: $(OBJS)
	$(CC) -o build/craped $(OBJS) -lpthread
