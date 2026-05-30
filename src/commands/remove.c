#include "commands/remove.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"

#include <stdlib.h>
#include <string.h>

int is_already_removed(const char *path) {
    int count = 0;
    IndexRow *rows = repo_read_index(&count);

    int found = 0;
    for (int i = count - 1; i >= 0; i--) {
        if (strcmp(rows[i].path, path) == 0) {
            if (rows[i].mod == 'r') {
                found = 1;
            }
            break;
        }
    }

    free(rows);
    return found;
}

Result remove(char *file_name) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    char *normalized = normalize_path(file_name);

    if (strncmp(normalized, MINIGIT_DIR, 8) == 0) {
        return RESERVED_MINIGIT_PATH;
    }

    if (is_already_removed(normalized)) {
        return OK;
    }

    repo_write_index('r', normalized);

    return OK;
}
