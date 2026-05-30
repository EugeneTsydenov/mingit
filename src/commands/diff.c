#include "commands/diff.h"
#include "repo/repo.h"
#include "result/result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Result diff(const char *target_commit) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    if (target_commit == NULL) {
        return OK;
    }

    CommitMeta target_meta;
    if (!repo_read_meta(target_commit, &target_meta)) {
        return COMMIT_NOT_FOUND;
    }

    char head_hash[17];
    repo_read_head(head_hash);

    int curr_count = 0, curr_capacity = 0;
    TrackedFile *curr_files =
        repo_read_tracked_files(head_hash, &curr_count, &curr_capacity);

    int target_count = 0, target_capacity = 0;
    TrackedFile *target_files =
        repo_read_tracked_files(target_commit, &target_count, &target_capacity);

    printf("Diff between HEAD (%s) and commit %s:\n\n", head_hash,
           target_commit);

    // Ищем измененные и добавленные (есть в HEAD, но отличаются или отсутствуют
    // в Target)
    for (int i = 0; i < curr_count; i++) {
        int found = 0;
        for (int j = 0; j < target_count; j++) {
            if (strcmp(curr_files[i].path, target_files[j].path) == 0) {
                found = 1;
                if (strcmp(curr_files[i].hash, target_files[j].hash) != 0) {
                    printf("\033[33mModified:\033[0m %s\n", curr_files[i].path);
                    printf("  HEAD:   %s\n", curr_files[i].hash);
                    printf("  Commit: %s\n\n", target_files[j].hash);
                }
                break;
            }
        }
        if (!found) {
            printf("\033[32mAdded:\033[0m    %s\n", curr_files[i].path);
            printf("  HEAD:   %s\n", curr_files[i].hash);
            printf("  Commit: <none>\n\n");
        }
    }

    // Ищем удаленные файлы (есть в Target, но уже удалены из HEAD)
    for (int i = 0; i < target_count; i++) {
        int found = 0;
        for (int j = 0; j < curr_count; j++) {
            if (strcmp(target_files[i].path, curr_files[j].path) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("\033[31mDeleted:\033[0m  %s\n", target_files[i].path);
            printf("  HEAD:   <none>\n");
            printf("  Commit: %s\n\n", target_files[i].hash);
        }
    }

    free(curr_files);
    free(target_files);

    return OK;
}
