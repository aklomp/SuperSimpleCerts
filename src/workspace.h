struct workspace
{
	struct cert *certs;
	GtkTreeIter popupIter;
};

struct workspace *workspace_new (void);
bool workspace_dirty (struct workspace *ws);
bool workspace_save (struct workspace *ws);
void workspace_close (struct workspace **ws);

struct cert *workspace_add_selfsigned_ca (struct workspace *ws);
struct cert *workspace_add_selfsigned (struct workspace *ws);
struct cert *workspace_add_child (struct workspace *ws, GtkTreeIter *parent);
void workspace_delete_cert (struct workspace *ws, GtkTreeIter *cert);
