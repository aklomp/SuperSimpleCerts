#include <stdbool.h>
#include <stdlib.h>

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
destroy (struct workspace *ws)
{
	if (ws == NULL) {
		return;
	}
	free(ws->certs);
	free(ws);
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
