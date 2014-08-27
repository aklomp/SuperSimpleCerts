#include <stdbool.h>
#include <stdlib.h>
#include <zlib.h>
#include <gtk/gtk.h>

#include "mainwindow.h"
#include "treestore.h"
#include "treeview.h"
#include "workspace.h"

static bool
save_workspace_dialog ()
{
	// Run a dialog window asking if the user wants to save the workspace:
	return true;
}

static bool
clearout_workspace (struct workspace **ws)
{
	// Close and destroy the current workspace;
	// returns true on success, false on failure:
	if (*ws == NULL) {
		return true;
	}
	if (workspace_dirty(*ws) && save_workspace_dialog()) {
		if (workspace_save(*ws) == false) {
			return false;
		}
	}
	workspace_close(ws);
	return true;
}

void
on_destroy (GtkWidget *object, struct workspace **ws)
{
	(void)object;

	if (clearout_workspace(ws) == false) {
		return;
	}
	gtk_main_quit();
}

void
on_menu_new (GtkImageMenuItem *item, struct workspace **ws)
{
	(void)item;

	if (clearout_workspace(ws) == false) {
		return;
	}
	if ((*ws = workspace_new()) == NULL) {
		return;
	}
}

void
on_menu_open (GtkImageMenuItem *item, struct workspace **ws)
{
	(void)item;

	if (clearout_workspace(ws) == false) {
		return;
	}
}

void
on_menu_save (GtkImageMenuItem *item, struct workspace **ws)
{
	(void)item;

	if (workspace_save(*ws) == false) {
		return;
	}
}

void
on_menu_saveas (GtkImageMenuItem *item, struct workspace **ws)
{
	(void)item;
	(void)ws;
}

void
on_menu_about (GtkImageMenuItem *item, struct workspace **ws)
{
	(void)item;
	(void)ws;
}

static char *
mainwindow_load ()
{
	// These symbols refer to the embedded XML interface definition string:
	extern char _binary_mainwindow_gz_start[];
	extern char _binary_mainwindow_gz_end[];

	char *buf;

	// We know the size of the uncompressed XML interface definition
	// string, because it's written to mainwindow.h at compile time:
	if ((buf = malloc(MAINWINDOW_SIZE)) == NULL) {
		return NULL;
	}
	// Initialize zlib control structure:
	z_stream stream =
		{ .zalloc = Z_NULL
		, .zfree = Z_NULL
		, .opaque = Z_NULL
		, .avail_in = _binary_mainwindow_gz_end - _binary_mainwindow_gz_start
		, .next_in = (Bytef *)_binary_mainwindow_gz_start
		, .avail_out = MAINWINDOW_SIZE
		, .next_out = (Bytef *)buf
		} ;

	// Must init with extra window size for gzip file format:
	if (inflateInit2(&stream, 16 | MAX_WBITS) != Z_OK) {
		free(buf);
		return NULL;
	}
	// A single pass should be enough:
	if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
		free(buf);
		return NULL;
	}
	inflateEnd(&stream);
	return buf;
}

int
ui_main (int argc, char **argv, char *app_name)
{
	char *mainwindow;
	struct workspace *ws = NULL;

	gtk_init(&argc, &argv);

	// Load the XML interface description string:
	if ((mainwindow = mainwindow_load()) == NULL) {
		return 1;
	}
	// Feed it to the builder:
	GtkBuilder *builder = gtk_builder_new();
	if (gtk_builder_add_from_string(builder, mainwindow, MAINWINDOW_SIZE, NULL) == 0) {
		free(mainwindow);
		return 1;
	}
	// Hookup signals, pass the workspace to every handler:
	gtk_builder_connect_signals(builder, &ws);

	// Fetch handle to main window:
	GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	gtk_window_set_title(GTK_WINDOW(window), app_name);

	treestore_init(builder);
	treeview_init(builder);

	// Destroy builder, show window, run event loop:
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);

	gtk_main();

	free(mainwindow);
	return 0;
}
