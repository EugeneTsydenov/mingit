#include "commands/add.h"
#include "fs/fs.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"
#include <string.h>

Result add(char *file_name) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    char *normalized = normalize_path(file_name);

    if (!check_file(normalized)) {
        return FILE_NOT_FOUND;
    }

    if (strncmp(normalized, MINIGIT_DIR, 8) == 0) {
        return RESERVED_MINIGIT_PATH;
    }

    repo_write_index('a', normalized);

    return OK;
}
