CFLAGS += -std=c99 -Werror -Wall -Wextra -pedantic
LDFLAGS += -Wl,--export-dynamic -lz

GTK_CFLAGS  = `pkg-config --cflags gtk+-3.0`
GTK_LDFLAGS = `pkg-config --libs gtk+-3.0`

.PHONY: clean
.INTERMEDIATE: mainwindow.gz mainwindow.h

PROG = supersimplecerts
OBJS = main.o mainwindow.o treestore.o treeview.o ui.o workspace.o

$(PROG): $(OBJS)
	$(CC) $(GTK_LDFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $^ -o $@

# Use linker to generate an "object file" containing the XML interface
# definition as a string:
mainwindow.o: mainwindow.gz
	$(LD) --relocatable --format=binary $^ -o $@

mainwindow.gz: mainwindow.glade
	gzip -9 < $^ > $@

mainwindow.h: mainwindow.glade
	echo "#define MAINWINDOW_SIZE `wc -c $^ | cut -f1 -d\ `" > $@

treestore.o: treestore.c
	$(CC) $(GTK_CFLAGS) $(CFLAGS) -c $^ -o $@

treeview.o: treeview.c
	$(CC) $(GTK_CFLAGS) $(CFLAGS) -c $^ -o $@

ui.o: ui.c mainwindow.h
	$(CC) $(GTK_CFLAGS) $(CFLAGS) -c ui.c -o $@

workspace.o: workspace.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(PROG)
