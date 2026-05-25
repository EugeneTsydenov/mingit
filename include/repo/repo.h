#ifndef REPO_H
#define REPO_H

#include <time.h>
#define MINIGIT_DIR ".minigit"

typedef struct {
    char path[512];
    char hash[17];
} TrackedFile;

typedef struct {
    char mod;
    char path[512];
} IndexRow;

typedef struct {
    char hash[17];
    char msg[512];
    time_t timestamp;
    char parent[17];
} CommitMeta;

int repo_create_directory(const char *path);

void repo_create_file(const char *fileName);

void repo_create_commit(const char *hash);

int repo_exists_local(void);

int repo_write_meta(const char *commit_hash, const char *parent,
                    const char *message, time_t timestamp);

int repo_write_head(const char *hash);

int repo_write_index(char operation, const char *file_name);

int mkdir_p(const char *path);

int repo_index_exists();

int repo_head_exists();

void repo_read_head(char *hash_buf);

TrackedFile *repo_read_tracked_files(const char *commit_hash, int *out_count, int *out_capaciry);

IndexRow *repo_read_index(int *out_count);

int repo_write_copy_files(const char *commit_hash, char *file_path);

int repo_write_tracked_files(const char *commit_hash, TrackedFile *files,
                             int count);

int repo_read_meta(const char *commit_hash, CommitMeta *meta);

#endif
