void treestore_init (GtkBuilder *builder);
void treestore_append_root (char *);
bool treestore_append_child (GtkTreeIter *parent, GtkTreeIter *child, char *);
void treestore_empty (void);
