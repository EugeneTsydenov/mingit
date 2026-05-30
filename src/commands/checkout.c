#include "commands/checkout.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Result checkout(const char *commit, const char *file_name) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    CommitMeta meta;
    if (!repo_read_meta(commit, &meta)) {
        return COMMIT_NOT_FOUND;
    }

    int count = 0;
    int capacity = 0;
    TrackedFile *files = repo_read_tracked_files(commit, &count, &capacity);
    
    int found = 0;
    char target_hash[17];
    for (int i = 0; i < count; i++) {
        if (strcmp(files[i].path, file_name) == 0) {
            found = 1;
            strcpy(target_hash, files[i].hash);
            break;
        }
    }
    
    free(files);
    
    if (!found) {
        return FILE_NOT_FOUND;
    }

    char src_path[512];
    snprintf(src_path, sizeof(src_path), "%s/commits/%s/files/%s", MINIGIT_DIR, target_hash, file_name);
    
    if (!copy_file(src_path, file_name)) {
        return FILE_NOT_FOUND;
    }

    return OK;
}
