#include "commands/status.h"
#include "repo/repo.h"
#include "result/result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Result status(void) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    if (!repo_index_exists()) {
        printf("Nothing to commit\n");
        return OK;
    }

    int index_count = 0;
    IndexRow *index_rows = repo_read_index(&index_count);

    char head_hash[17] = {0};
    repo_read_head(head_hash);

    int parent_file_count = 0;
    int parent_file_capacity = 0;
    TrackedFile *parent_files = repo_read_tracked_files(head_hash, &parent_file_count, &parent_file_capacity);



    printf("Changes to be committed:\n\n");

    for (int i = 0; i < index_count; i++) {
        if (index_rows[i].mod == 'r') {
            printf("\033[31mDeleted:\033[0m  %s\n", index_rows[i].path);
        } else if (index_rows[i].mod == 'a') {
            int found_in_parent = 0;
            if (parent_files) {
                for (int j = 0; j < parent_file_count; j++) {
                    if (strcmp(parent_files[j].path, index_rows[i].path) == 0) {
                        found_in_parent = 1;
                        break;
                    }
                }
            }

            if (found_in_parent) {
                printf("\033[33mModified:\033[0m %s\n", index_rows[i].path);
            } else {
                printf("\033[32mCreated:\033[0m  %s\n", index_rows[i].path);
            }
        }
    }
    printf("\n");
    
    free(index_rows);
    free(parent_files);

    return OK;
}
