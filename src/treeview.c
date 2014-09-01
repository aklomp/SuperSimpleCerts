#include <stdbool.h>
#include <gtk/gtk.h>

#include "cert.h"
#include "treestore.h"
#include "workspace.h"

static GtkTreeViewColumn *column = NULL;
static GtkMenu *popupmenublank = NULL;
static GtkMenu *popupmenuca = NULL;

static void
show_popup_menu_blank (GdkEventButton *event)
{
	gtk_menu_popup(popupmenublank, NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}

static void
show_popup_menu_ca (GdkEventButton *event)
{
	gtk_menu_popup(popupmenuca, NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}

static void
on_leftclick (GtkTreeView *treeview, GdkEventButton *event, gboolean got_iter, struct workspace **ws)
{
	(void)treeview;
	(void)event;
	(void)got_iter;
	(void)ws;
}

static void
on_rightclick (GtkTreeView *treeview, GdkEventButton *event, gboolean got_iter, struct workspace **ws)
{
	struct cert *cert;

	(void)treeview;

	// Not on a path: show generic workspace popup menu:
	if (got_iter == FALSE) {
		show_popup_menu_blank(event);
		return;
	}
	// Fetch the certificate:
	if ((cert = treestore_cert_from_iter(&(*ws)->popupIter)) == NULL) {
		return;
	}
	if (cert->is_ca) {
		show_popup_menu_ca(event);
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
on_popupmenu_ca_create_child (GtkMenuItem *menuitem, struct workspace **ws)
{
	(void)menuitem;

	// The iter on which to operate was saved in the workspace:
	workspace_add_child(*ws, &(*ws)->popupIter);
}

void
on_popupmenu_ca_delete (GtkMenuItem *menuitem, struct workspace **ws)
{
	(void)menuitem;

	// The iter on which to operate was saved in the workspace:
	workspace_delete_cert(*ws, &(*ws)->popupIter);
}

void
on_treeview_button_press (GtkTreeView *treeview, GdkEventButton *event, struct workspace **ws)
{
	if (event->type != GDK_BUTTON_PRESS) {
		return;
	}
	GtkTreePath *path;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
	gboolean got_iter;

	gtk_tree_selection_unselect_all(selection);

	// Get path at click position:
	if ((got_iter = gtk_tree_view_get_path_at_pos(treeview, event->x, event->y, &path, NULL, NULL, NULL)))
	{
		// Select the row we clicked on:
		gtk_tree_selection_select_path(selection, path);

		// Use the workspace to store the iter on which to popup:
		treestore_get_iter(path, &(*ws)->popupIter);

		// Release path:
		gtk_tree_path_free(path);
	}
	// Dispatch event:
	switch (event->button) {
		case 1: on_leftclick(treeview, event, got_iter, ws); break;
		case 3: on_rightclick(treeview, event, got_iter, ws); break;
	}
}

void
on_treeview_popup_menu (GtkTreeView *treeview, struct workspace **ws)
{
	// Handler for the "popup-menu" signal on the Treeview.
	// Check if a row is selected:
	GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
	gboolean got_iter;

	// Find the iter of the selected item, if any:
	if ((got_iter = (gtk_tree_selection_count_selected_rows(selection) > 0))) {
		gtk_tree_selection_get_selected(selection, NULL, &(*ws)->popupIter);
	}
	on_rightclick(treeview, NULL, got_iter, ws);
}

void
treeview_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "treeviewcolumn"));
	popupmenublank = GTK_MENU(gtk_builder_get_object(builder, "popupmenublank"));
	popupmenuca = GTK_MENU(gtk_builder_get_object(builder, "popupmenuca"));

	// Add text renderer for first column:
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", 0);
}
