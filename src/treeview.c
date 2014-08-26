#include <gtk/gtk.h>

static GtkTreeViewColumn *column = NULL;

void
treeview_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "treeviewcolumn"));

	// Add text renderer for first column:
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", 0);
}
