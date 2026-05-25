#include "commands/remove.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"

#include <string.h>

Result remove(char *file_name) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    char *normalized = normalize_path(file_name);

    if (strncmp(normalized, MINIGIT_DIR, 8) == 0) {
        return RESERVED_MINIGIT_PATH;
    }

    repo_write_index('r', normalized);

    return OK;
}
