#include <gtk/gtk.h>

static GtkTreeStore *treestore = NULL;

static void
append_child (GtkTreeIter *parent, GtkTreeIter *child, const char *name)
{
	gtk_tree_store_append(treestore, child, parent);
	gtk_tree_store_set
		( treestore
		, child
		, 0, name
		, -1
		) ;
}

void
treestore_empty (void)
{
	gtk_tree_store_clear(treestore);
}

void
treestore_append_root (char *name)
{
	GtkTreeIter iter;
	append_child(NULL, &iter, name);
}

void
treestore_append_child (GtkTreeIter *parent, GtkTreeIter *child, char *name)
{
	append_child(parent, child, name);
}

void
treestore_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	treestore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treestore"));
}
