void treestore_init (GtkBuilder *builder);
void treestore_append_root (GtkTreeIter *child, const struct cert *);
void treestore_append_child (GtkTreeIter *parent, GtkTreeIter *child, const struct cert *);
void treestore_empty (void);
void treestore_foreach_cert (void (*callback)(struct cert *));
