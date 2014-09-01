#include <stdbool.h>
#include <string.h>
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

struct cert *
treestore_cert_from_iter (GtkTreeIter *iter)
{
	struct cert *cert;
	gtk_tree_model_get(GTK_TREE_MODEL(treestore), iter, 1, &cert, -1);
	return cert;
}

struct data_find_cert
{
	const struct cert *cert;
	GtkTreeIter *iter;
	bool found;
};

static gboolean
callback_find_cert (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, void *data)
{
	struct data_find_cert *userdata = data;

	(void)model;
	(void)path;

	if (treestore_cert_from_iter(iter) == userdata->cert) {
		memcpy(userdata->iter, iter, sizeof(*iter));
		userdata->found = true;
		return TRUE;
	}
	return FALSE;
}

bool
treestore_find_cert (const struct cert *cert, GtkTreeIter *iter)
{
	struct data_find_cert userdata = { .cert = cert, .iter = iter, .found = false };
	gtk_tree_model_foreach(GTK_TREE_MODEL(treestore), (GtkTreeModelForeachFunc)callback_find_cert, &userdata);
	return userdata.found;
}

struct data_foreach_cert
{
	void (*callback)(struct cert *);
};

static gboolean
callback_foreach_cert (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, void *data)
{
	struct data_foreach_cert *userdata = data;

	(void)model;
	(void)path;
	userdata->callback(treestore_cert_from_iter(iter));

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
