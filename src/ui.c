#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <zlib.h>
#include <gtk/gtk.h>

#include "cert.h"
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
mainwindow_load (uint32_t *mainwindow_size)
{
	// These symbols refer to the embedded XML interface definition string:
	extern char _binary_mainwindow_gz_start[];
	extern char _binary_mainwindow_gz_end[];

	char *buf;

	// The uncompressed size of a gzip stream is stored in little-endian
	// format in the last four bytes:
	*mainwindow_size = *(((uint32_t *)_binary_mainwindow_gz_end) - 1);

	if ((buf = malloc(*mainwindow_size)) == NULL) {
		return NULL;
	}
	// Initialize zlib control structure:
	z_stream stream =
		{ .zalloc = Z_NULL
		, .zfree = Z_NULL
		, .opaque = Z_NULL
		, .avail_in = _binary_mainwindow_gz_end - _binary_mainwindow_gz_start
		, .next_in = (Bytef *)_binary_mainwindow_gz_start
		, .avail_out = *mainwindow_size
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
	uint32_t mainwindow_size;

	gtk_init(&argc, &argv);

	// Load the XML interface description string:
	if ((mainwindow = mainwindow_load(&mainwindow_size)) == NULL) {
		return 1;
	}
	// Feed it to the builder:
	GtkBuilder *builder = gtk_builder_new();
	if (gtk_builder_add_from_string(builder, mainwindow, mainwindow_size, NULL) == 0) {
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

	// Show window:
	gtk_widget_show(window);

	// Run event loop:
	gtk_main();

	// Wait until the event loop returns to destroy the builder;
	// we need the reference to stay live for popup menus and subwindows:
	g_object_unref(G_OBJECT(builder));

	free(mainwindow);
	return 0;
}
