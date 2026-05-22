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

    size_t sub_count = sizeof(sub_names) / sizeof(sub_names[0]);

    for (size_t i = 0; i < sub_count; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", MINIGIT_DIR,
                 sub_names[i]);
        create_path(full_path);
    }

    const char *file_names[] = {"HEAD", "index"};

    size_t file_count = sizeof(file_names) / sizeof(file_names[0]);

    for (size_t i = 0; i < file_count; i++) {
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
