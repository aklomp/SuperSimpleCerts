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
	treestore_foreach_cert(callback_destroy_cert);
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
