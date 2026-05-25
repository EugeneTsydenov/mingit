#include "fs/fs.h"

#include <stdio.h>
#include <sys/stat.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define make_directory(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define make_directory(dir) mkdir(dir, 0755)
#endif

int check_file(char *file_name) {
    struct stat buffer;

    if (stat(file_name, &buffer) != 0) {
        return 0;
    }

    if (S_ISREG(buffer.st_mode)) {
        return 1;
    }

    return 0;
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
