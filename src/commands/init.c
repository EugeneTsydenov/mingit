#include "commands/init.h"
#include "repo/repo.h"
#include "result/result.h"
#include <stdio.h>

Result init(void) {
    if (repo_exists_local()) {
        return REPO_ALREADY_EXISTS;
    }

    make_dir(MINIGIT_DIR);

    const char *sub_names[] = {"commits", "objects"};
    char full_path[512];

    for (int i = 0; i < 3; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", MINIGIT_DIR,
                 sub_names[i]);
        create_path(full_path);
    }

    const char *file_names[] = {"HEAD", "index"};

    for (int i = 0; i < 2; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", MINIGIT_DIR,
                 file_names[i]);
        FILE *file = fopen(full_path, "w");
        if (file) {
            fclose(file);
        }
    }

    printf("initialized empty minigit repository.\n");
    return OK;
}
