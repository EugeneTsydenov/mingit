#include "repo/repo.h"
#include "fs/fs.h"
#include "util/util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#define stat _stat
#else
#include <unistd.h>
#endif

void repo_build_pathf(char *out, size_t size, const char *format, ...) {
    char relative[512];

    va_list args;

    va_start(args, format);

    vsnprintf(relative, sizeof(relative), format, args);

    va_end(args);

    snprintf(out, size, "%s/%s", MINIGIT_DIR, relative);
}

int repo_create_directory(const char *path) {
    char full_path[512];

    repo_build_pathf(full_path, sizeof(full_path), "%s", path);

    return mkdir_p(full_path);
}

void repo_create_file(const char *file_name) {
    char full_path[512];

    repo_build_pathf(full_path, sizeof(full_path), "%s", file_name);

    FILE *file = fopen(full_path, "w");

    if (file) {
        fclose(file);
    }
}

void repo_create_commit(const char *hash) {
    repo_create_directory("commits");

    char path[512];

    snprintf(path, sizeof(path), "commits/%s", hash);
    repo_create_directory(path);

    snprintf(path, sizeof(path), "commits/%s/files", hash);
    repo_create_directory(path);

    snprintf(path, sizeof(path), "commits/%s/meta", hash);
    repo_create_file(path);

    snprintf(path, sizeof(path), "commits/%s/tracked_files", hash);
    repo_create_file(path);
}

int repo_exists_local(void) {
    struct stat stats;

    return stat(MINIGIT_DIR, &stats) == 0 && S_ISDIR(stats.st_mode);
}

int repo_write_meta(const char *commit_hash, const char *parent,
                    const char *message, time_t timestamp) {
    char path[512];

    repo_build_pathf(path, sizeof(path), "commits/%s/meta", commit_hash);

    FILE *file = fopen(path, "w");

    if (!file) {
        return 0;
    }

    fprintf(file, "parent=%s\n", parent);
    fprintf(file, "message=%s\n", message);
    fprintf(file, "time=%lld\n", (long long)timestamp);
    fprintf(file, "hash=%s\n", commit_hash);

    fclose(file);

    return 1;
}

int repo_write_head(const char *hash) {
    char path[512];

    repo_build_pathf(path, sizeof(path), "HEAD");

    FILE *file = fopen(path, "w");

    if (!file) {
        return 0;
    }

    fprintf(file, "%s\n", hash);

    fclose(file);

    return 1;
}

int repo_write_index(char operation, const char *file_name) {
    char path[512];

    repo_build_pathf(path, sizeof(path), "index");

    FILE *file = fopen(path, "a");

    if (!file) {
        return 0;
    }

    fprintf(file, "%c %s\n", operation, file_name);

    fclose(file);

    return 1;
}

int repo_index_exists() {
    struct stat buffer;
    char path[512];
    repo_build_pathf(path, sizeof(path), "index");
    return (stat(path, &buffer) == 0) && S_ISREG(buffer.st_mode);
}

int repo_head_exists() {
    struct stat buffer;
    char path[512];
    repo_build_pathf(path, sizeof(path), "HEAD");
    return (stat(path, &buffer) == 0) && S_ISREG(buffer.st_mode);
}

void repo_read_head(char *hash_buf) {
    char path[512];
    repo_build_pathf(path, sizeof(path), "HEAD");

    FILE *file = fopen(path, "r");
    if (!file) {
        return;
    }

    if (fscanf(file, "%16s", hash_buf) != 1) {
        fclose(file);
        return;
    }

    fclose(file);
}

TrackedFile *repo_read_tracked_files(const char *commit_hash, int *out_count,
                                     int *out_capacity) {
    *out_count = 0;

    char path[512];
    repo_build_pathf(path, sizeof(path), "commits/%s/tracked_files",
                     commit_hash);

    FILE *file = fopen(path, "r");
    if (!file) {
        return NULL;
    }

    *out_capacity = 10;
    TrackedFile *files = malloc(*out_capacity * sizeof(TrackedFile));

    int count = 0;
    while (fscanf(file, "%511s %16s", files[count].path, files[count].hash) ==
           2) {
        count++;

        if (count >= *out_capacity) {
            *out_capacity *= 2;
            TrackedFile *new_files =
                realloc(files, *out_capacity * sizeof(TrackedFile));
            files = new_files;
        }
    }

    fclose(file);
    *out_count = count;
    return files;
}

IndexRow *repo_read_index(int *out_count) {
    *out_count = 0;

    char path[512];
    repo_build_pathf(path, sizeof(path), "index");

    FILE *file = fopen(path, "r");
    if (!file) {
        return NULL;
    }

    int capacity = 7;
    IndexRow *rows = malloc(capacity * sizeof(IndexRow));

    int count = 0;
    while (fscanf(file, "%c %s", &rows[count].mod, rows[count].path)) {
        count++;

        if (count >= capacity) {
            capacity *= 2;
            IndexRow *new_rows = realloc(rows, capacity * sizeof(IndexRow));
            rows = new_rows;
        }
    }

    fclose(file);
    *out_count = count;

    return rows;
}

int repo_write_copy_files(const char *commit_hash, char *file_path) {
    char path[512];
    repo_build_pathf(path, sizeof(path), "commits/%s/files/%s", commit_hash,
                     file_path);

    char dir_path[512];
    strcpy(dir_path, path);
    char *last_slash = strrchr(dir_path, '/');
    if (last_slash) {
        *last_slash = '\0';
        mkdir_p(dir_path);
    }

    if (!copy_file(file_path, path)) {
        return 0;
    }
    return 1;
}

int repo_write_tracked_files(const char *commit_hash, TrackedFile *files,
                             int count) {
    char path[512];
    repo_build_pathf(path, sizeof(path), "commits/%s/tracked_files",
                     commit_hash);

    FILE *file = fopen(path, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s\n", files[i].path, files[i].hash);
    }

    return 1;
}
