void treestore_init (GtkBuilder *builder);
struct cert *treestore_cert_from_iter (GtkTreeIter *iter);
void treestore_append_root (GtkTreeIter *child, const struct cert *);
void treestore_append_child (GtkTreeIter *parent, GtkTreeIter *child, const struct cert *);
void treestore_empty (void);
bool treestore_find_cert (const struct cert *cert, GtkTreeIter *iter);
void treestore_foreach_cert (struct cert *parent, void (*callback)(struct cert *));
