#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cert.h"

static struct cert *
create (struct cert *parent, bool is_selfsigned, bool is_ca)
{
	struct cert *cert;

	if ((cert = malloc(sizeof(*cert))) == NULL) {
		return NULL;
	}
	cert->displayname = NULL;
	cert->key = NULL;
	cert->serial = NULL;
	cert->not_before = NULL;
	cert->not_after = NULL;
	cert->parent = parent;
	cert->is_selfsigned = is_selfsigned;
	cert->is_ca = is_ca;

	return cert;
}

static void
destroy (struct cert *cert)
{
	if (cert == NULL) {
		return;
	}
	free(cert->displayname);
	free(cert->key);
	free(cert->serial);
	free(cert->not_before);
	free(cert->not_after);
	free(cert);
}

struct cert *
cert_new (struct cert *parent, bool is_selfsigned, bool is_ca)
{
	return create(parent, is_selfsigned, is_ca);
}

void
cert_destroy (struct cert **cert)
{
	if (cert == NULL) {
		return;
	}
	destroy(*cert);
	*cert = NULL;
}

void
cert_set_displayname (struct cert *cert, const char *const displayname)
{
	size_t len = strlen(displayname) + 1;

	if (cert->displayname != NULL) {
		free(cert->displayname);
	}
	if ((cert->displayname = malloc(len)) == NULL) {
		return;
	}
	memcpy(cert->displayname, displayname, len);
}
