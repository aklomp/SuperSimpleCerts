#include <stdbool.h>
#include <gtk/gtk.h>

#include "cert.h"
#include "workspace.h"

static GtkTreeViewColumn *column = NULL;
static GtkMenu *popupmenublank = NULL;

static void
show_popup_menu_blank (GdkEventButton *event)
{
	gtk_menu_popup(popupmenublank, NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}

static void
on_leftclick (GtkTreeView *treeview, GdkEventButton *event, gboolean got_path, GtkTreePath *path)
{
	(void)treeview;
	(void)event;
	(void)got_path;
	(void)path;
}

static void
on_rightclick (GtkTreeView *treeview, GdkEventButton *event, gboolean got_path, GtkTreePath *path)
{
	(void)treeview;
	(void)event;
	(void)got_path;
	(void)path;

	// Not on a path: show generic workspace popup menu:
	if (got_path == FALSE) {
		show_popup_menu_blank(event);
		return;
	}
}

void
on_popupmenu_new_ca (GtkMenuItem *menuitem, struct workspace **ws)
{
	struct cert *cert;
	(void)menuitem;

	if (*ws == NULL && (*ws = workspace_new()) == NULL) {
		return;
	}
	if ((cert = workspace_add_selfsigned_ca(*ws)) == NULL) {
		return;
	}
}

void
on_popupmenu_new_ssc (GtkMenuItem *menuitem, struct workspace **ws)
{
	struct cert *cert;
	(void)menuitem;

	if (*ws == NULL && (*ws = workspace_new()) == NULL) {
		return;
	}
	if ((cert = workspace_add_selfsigned(*ws)) == NULL) {
		return;
	}
}

void
on_treeview_button_press (GtkTreeView *treeview, GdkEventButton *event)
{
	if (event->type != GDK_BUTTON_PRESS) {
		return;
	}
	GtkTreePath *path;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
	gboolean got_path;

	gtk_tree_selection_unselect_all(selection);

	// Get path at click position, select it:
	if ((got_path = gtk_tree_view_get_path_at_pos(treeview, event->x, event->y, &path, NULL, NULL, NULL))) {
		gtk_tree_selection_select_path(selection, path);
	}
	// Dispatch event:
	switch (event->button) {
		case 1: on_leftclick(treeview, event, got_path, path); break;
		case 3: on_rightclick(treeview, event, got_path, path); break;
	}
	gtk_tree_path_free(path);
}

void
on_treeview_popup_menu (void)
{
	// Handler for the "popup-menu" signal on the Treeview:
	show_popup_menu_blank(NULL);
}

void
treeview_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "treeviewcolumn"));
	popupmenublank = GTK_MENU(gtk_builder_get_object(builder, "popupmenublank"));

	// Add text renderer for first column:
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", 0);
}
