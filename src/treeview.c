#include <stdbool.h>
#include <gtk/gtk.h>

#include "cert.h"
#include "treestore.h"
#include "workspace.h"

static GtkTreeView *treeview = NULL;
static GtkTreeViewColumn *column = NULL;
static GtkMenu *popupmenublank = NULL;
static GtkMenu *popupmenuca = NULL;
static GtkMenu *popupmenussc = NULL;
static GtkMenu *popupmenuchild = NULL;

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
	GtkMenu *menu = popupmenublank;

	(void)treeview;

	if (got_iter != FALSE)
	{
		// Fetch the certificate:
		if ((cert = treestore_cert_from_iter(&(*ws)->popupIter)) == NULL) {
			return;
		}
		if (cert->is_ca) {
			menu = popupmenuca;
		}
		else if (cert->is_selfsigned) {
			menu = popupmenussc;
		}
		else {
			menu = popupmenuchild;
		}
	}
	gtk_menu_popup(menu, NULL, NULL, NULL, NULL, (event != NULL) ? event->button : 0, gdk_event_get_time((GdkEvent*)event));
}

void
on_popupmenu_new_ca (GtkMenuItem *menuitem, struct workspace **ws)
{
	struct cert *cert;
	(void)menuitem;

	if (*ws == NULL && !(*ws = workspace_new()))
		return;

	if (!(cert = workspace_add_selfsigned_ca(*ws)))
		return;
}

void
on_popupmenu_new_ssc (GtkMenuItem *menuitem, struct workspace **ws)
{
	struct cert *cert;
	(void)menuitem;

	if (*ws == NULL && !(*ws = workspace_new()))
		return;

	if (!(cert = workspace_add_selfsigned(*ws)))
		return;
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
on_popupmenu_ssc_delete (GtkMenuItem *menuitem, struct workspace **ws)
{
	(void)menuitem;

	// The iter on which to operate was saved in the workspace:
	workspace_delete_cert(*ws, &(*ws)->popupIter);
}

void
on_popupmenu_child_delete (GtkMenuItem *menuitem, struct workspace **ws)
{
	(void)menuitem;

	// The iter on which to operate was saved in the workspace:
	workspace_delete_cert(*ws, &(*ws)->popupIter);
}

void
on_treeview_button_press (GtkTreeView *treeview, GdkEventButton *event, struct workspace **ws)
{
	GtkTreePath *path;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
	gboolean got_iter;

	gtk_tree_selection_unselect_all(selection);

	// Get path at click position:
	if ((got_iter = gtk_tree_view_get_path_at_pos(treeview, event->x, event->y, &path, &column, NULL, NULL)))
	{
		// Select the row we clicked on:
		gtk_tree_selection_select_path(selection, path);

		// Is this a left double-click on a path?
		if (event->type == GDK_2BUTTON_PRESS && event->button == 1)
		{
			// Send activation signal for row:
			gtk_tree_view_row_activated(treeview, path, column);
			gtk_tree_path_free(path);
			return;
		}
		// Use the workspace to store the iter on which to popup:
		treestore_get_iter(path, &(*ws)->popupIter);

		// Release path:
		gtk_tree_path_free(path);
	}

	// Dispatch event:
	switch (event->button) {
	case 1:
		on_leftclick(treeview, event, got_iter, ws);
		break;

	case 3:
		on_rightclick(treeview, event, got_iter, ws);
		break;
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
on_treeview_row_activated (GtkTreeView *treeview, GtkTreePath *path)
{
	// This signal is raised when a user double-clicks on a row;
	// toggle row collapse:

	if (gtk_tree_view_row_expanded(treeview, path)) {
		gtk_tree_view_collapse_row(treeview, path);
		return;
	}
	gtk_tree_view_expand_row(treeview, path, TRUE);
}

void
treeview_expand_iter (GtkTreeIter *iter)
{
	// Find path corresponding to iter, expand view till there:
	GtkTreePath *path = gtk_tree_model_get_path(gtk_tree_view_get_model(treeview), iter);
	gtk_tree_view_expand_to_path(treeview, path);
	gtk_tree_path_free(path);
}

void
treeview_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	treeview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview"));
	column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "treeviewcolumn"));
	popupmenublank = GTK_MENU(gtk_builder_get_object(builder, "popupmenublank"));
	popupmenuca = GTK_MENU(gtk_builder_get_object(builder, "popupmenuca"));
	popupmenussc = GTK_MENU(gtk_builder_get_object(builder, "popupmenussc"));
	popupmenuchild = GTK_MENU(gtk_builder_get_object(builder, "popupmenuchild"));

	// Add text renderer for first column:
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", 0);
}
