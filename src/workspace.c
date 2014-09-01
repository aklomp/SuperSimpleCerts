#include <stdbool.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "cert.h"
#include "treestore.h"
#include "workspace.h"

static struct workspace *
create (void)
{
	struct workspace *ws;

	if ((ws = malloc(sizeof(*ws))) == NULL) {
		return NULL;
	}
	ws->certs = NULL;

	return ws;
}

static void
callback_destroy_cert (struct cert *cert)
{
	cert_destroy(&cert);
}

static void
destroy (struct workspace *ws)
{
	if (ws == NULL) {
		return;
	}
	treestore_foreach_cert(NULL, callback_destroy_cert);
	treestore_empty();
}

struct workspace *
workspace_new (void)
{
	struct workspace *ws;

	if ((ws = create()) == NULL) {
		return NULL;
	}
	return ws;
}

bool
workspace_dirty (struct workspace *ws)
{
	if (ws == NULL) {
		return false;
	}
	return true;
}

bool
workspace_save (struct workspace *ws)
{
	if (ws == NULL) {
		return true;
	}
	return true;
}

void
workspace_close (struct workspace **ws)
{
	if (ws == NULL) {
		return;
	}
	destroy(*ws);
	*ws = NULL;
}

struct cert *
workspace_add_selfsigned_ca (struct workspace *ws)
{
	// Add a new selfsigned CA template to the end of the root list:
	(void)ws;
	struct cert *cert;
	GtkTreeIter child;

	if ((cert = cert_new(NULL, true, true)) == NULL) {
		return NULL;
	}
	cert_set_displayname(cert, "New Selfsigned CA");
	treestore_append_root(&child, cert);
	return cert;
}

struct cert *
workspace_add_selfsigned (struct workspace *ws)
{
	// Add a new selfsigned template to the end of the root list;
	(void)ws;
	struct cert *cert;
	GtkTreeIter child;

	if ((cert = cert_new(NULL, true, false)) == NULL) {
		return NULL;
	}
	cert_set_displayname(cert, "New Selfsigned");
	treestore_append_root(&child, cert);
	return cert;
}

struct cert *
workspace_add_child (struct workspace *ws, GtkTreeIter *parentIter)
{
	// Add a new child below an existing certificate:
	(void)ws;

	struct cert *parent;
	struct cert *child;
	GtkTreeIter childIter;

	// Get parent cert:
	if ((parent = treestore_cert_from_iter(parentIter)) == NULL) {
		return NULL;
	}
	if ((child = cert_new(parent, false, false)) == NULL) {
		return NULL;
	}
	cert_set_displayname(child, "New Child");
	treestore_append_child(parentIter, &childIter, child);
	return child;
}

void
workspace_delete_cert (struct workspace *ws, GtkTreeIter *iter)
{
	struct cert *cert;

	if ((cert = treestore_cert_from_iter(iter)) == NULL) {
		return;
	}
	// If this is the first cert of the lot, clear the pointer:
	if (ws->certs == cert) {
		ws->certs = NULL;
	}
	// Recursively destroy cert and children; reset prev/next pointers:
	cert_destroy(&cert);
}
