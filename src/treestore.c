#include <stdbool.h>
#include <gtk/gtk.h>

#include "cert.h"

static GtkTreeStore *treestore = NULL;

static void
append_child (GtkTreeIter *parent, GtkTreeIter *child, const struct cert *cert)
{
	gtk_tree_store_append(treestore, child, parent);
	gtk_tree_store_set
		( treestore
		, child
		, 0, cert->displayname
		, 1, cert
		, -1
		) ;
}

void
treestore_empty (void)
{
	gtk_tree_store_clear(treestore);
}

struct data_foreach_cert
{
	void (*callback)(struct cert *);
};

static gboolean
callback_foreach_cert (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, void *data)
{
	struct cert *cert;
	struct data_foreach_cert *userdata = data;

	(void)path;
	gtk_tree_model_get(model, iter, 1, &cert, -1);
	userdata->callback(cert);
	return FALSE;
}

void
treestore_foreach_cert (void (*callback)(struct cert *))
{
	struct data_foreach_cert userdata = { .callback = callback };
	gtk_tree_model_foreach(GTK_TREE_MODEL(treestore), (GtkTreeModelForeachFunc)callback_foreach_cert, &userdata);
}

void
treestore_append_root (GtkTreeIter *child, const struct cert *cert)
{
	append_child(NULL, child, cert);
}

void
treestore_append_child (GtkTreeIter *parent, GtkTreeIter *child, const struct cert *cert)
{
	append_child(parent, child, cert);
}

void
treestore_init (GtkBuilder *builder)
{
	// Initialize our static variables from the Builder instance:
	treestore = GTK_TREE_STORE(gtk_builder_get_object(builder, "treestore"));
}
