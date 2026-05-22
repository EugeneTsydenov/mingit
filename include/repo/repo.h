#ifndef REPO_H
#define REPO_H

#define MINIGIT_DIR ".minigit"

void create_path(const char *path);

int repo_exists_local(void);

int make_dir(const char *path);

#endif
