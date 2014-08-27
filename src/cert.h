struct cert
{
	char *displayname;
	char *key;
	char *serial;
	char *not_before;
	char *not_after;

	// Parent certificate if not selfsigned:
	struct cert *parent;

	bool is_selfsigned;
	bool is_ca;
};

struct cert *cert_new (void);
void cert_destroy (struct cert **cert);
void cert_set_displayname (struct cert *cert, const char *const displayname);
