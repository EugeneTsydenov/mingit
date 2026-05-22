#include "fs/fs.h"

#include <sys/stat.h>

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
