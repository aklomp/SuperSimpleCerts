struct workspace
{
	char *certs;
};

struct workspace *workspace_new (void);
bool workspace_dirty (struct workspace *ws);
bool workspace_save (struct workspace *ws);
void workspace_close (struct workspace **ws);
