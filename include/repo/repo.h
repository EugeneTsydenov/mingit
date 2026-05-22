#ifndef REPO_H
#define REPO_H

#include <time.h>
#define MINIGIT_DIR ".minigit"

int repo_create_directory(const char *path);

void repo_create_file(const char *fileName);

void repo_create_commit(const char *hash);

int repo_exists_local(void);

int repo_write_meta(const char *commit_hash, const char *parent,
                    const char *message, time_t timestamp);

int repo_write_head(const char *hash);

int repo_write_index(char operation, const char *file_name);

int mkdir_p(const char *path);

#endif
