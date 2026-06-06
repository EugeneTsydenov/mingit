#include "commands/remove.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"

#include <stdlib.h>
#include <string.h>

static char get_last_index_mod(const char *path) {
    int count = 0;
    IndexRow *rows = repo_read_index(&count);

    char last_mod = 0;
    for (int i = count - 1; i >= 0; i--) {
        if (strcmp(rows[i].path, path) == 0) {
            last_mod = rows[i].mod;
            break;
        }
    }

    if (rows) {
        free(rows);
    }
    return last_mod;
}

static int is_tracked(const char *path) {
    if (!repo_head_exists()) {
        return 0;
    }

    char head_hash[17];
    repo_read_head(head_hash);

    int count = 0, capacity = 0;
    TrackedFile *files = repo_read_tracked_files(head_hash, &count, &capacity);
    
    int tracked = 0;
    if (files != NULL) {
        for (int i = 0; i < count; i++) {
            if (strcmp(files[i].path, path) == 0) {
                tracked = 1;
                break;
            }
        }
        free(files);
    }

    return tracked;
}

Result remove(char *file_name) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    char *normalized = normalize_path(file_name);

    if (strncmp(normalized, MINIGIT_DIR, 8) == 0) {
        return RESERVED_MINIGIT_PATH;
    }

    char last_mod = get_last_index_mod(normalized);
    
    if (!is_tracked(normalized) && last_mod != 'a' && last_mod != 'm') {
        return OK;
    }

    if (last_mod == 'r') {
        return OK;
    }

    repo_write_index('r', normalized);

    return OK;
}
