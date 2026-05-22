#include "repo/repo.h"

#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#define stat _stat
#else
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define make_directory(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define make_directory(dir) mkdir(dir, 0755)
#endif

void repo_build_pathf(char *out, size_t size, const char *format, ...) {
    char relative[512];

    va_list args;

    va_start(args, format);

    vsnprintf(relative, sizeof(relative), format, args);

    va_end(args);

    snprintf(out, size, "%s/%s", MINIGIT_DIR, relative);
}

int mkdir_p(const char *path) {
    char temp[512];

    snprintf(temp, sizeof(temp), "%s", path);

    for (char *p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            make_directory(temp);
            *p = '/';
        }
    }

    return make_directory(temp);
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

    fprintf(file, "%s %s\n", operation == 'a' ? "ADD" : "REMOVE", file_name);

    fclose(file);

    return 1;
}
