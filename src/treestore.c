#include <gtk/gtk.h>

static GtkTreeStore *treestore = NULL;

void
treestore_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	treestore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treestore"));
}
