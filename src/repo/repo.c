#include "repo/repo.h"

#include <stdio.h>
#include <sys/stat.h>

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

void create_path(const char *path) {
    char temp[256];
    char *p = NULL;

    snprintf(temp, sizeof(temp), "%s", path);

    for (p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            make_directory(temp);
            *p = '/';
        }
    }
    make_directory(temp);
}

int repo_exists_local(void) {
    struct stat stats;
    return stat(MINIGIT_DIR, &stats) == 0 && S_ISDIR(stats.st_mode);
}

int make_dir(const char *path) { return make_directory(path); }
