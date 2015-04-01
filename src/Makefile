CFLAGS += -std=c99 -Werror -Wall -Wextra -pedantic
LDFLAGS += -Wl,--export-dynamic -lz

GTK_CFLAGS  = `pkg-config --cflags gtk+-3.0`
GTK_LDFLAGS = `pkg-config --libs gtk+-3.0`

.PHONY: clean
.INTERMEDIATE: mainwindow.gz mainwindow.h

PROG = supersimplecerts

GTK_OBJS = \
  treestore.o \
  treeview.o \
  ui.o \
  workspace.o

OBJS = \
  cert.o \
  main.o

ALL_OBJS = $(OBJS) $(GTK_OBJS) mainwindow.o

$(PROG): $(ALL_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(GTK_LDFLAGS)

# Use linker to generate an "object file" containing the XML interface
# definition as a string:
mainwindow.o: mainwindow.gz
	$(LD) --relocatable --format=binary $^ -o $@

mainwindow.gz: mainwindow.glade
	gzip -9 < $^ > $@

mainwindow.h: mainwindow.glade
	echo "#define MAINWINDOW_SIZE `wc -c $^ | cut -f1 -d\ `" > $@

ui.o: ui.c mainwindow.h

$(GTK_OBJS): %.o: %.c
	$(CC) $(GTK_CFLAGS) $(CFLAGS) -c $< -o $@

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(ALL_OBJS) $(PROG)
