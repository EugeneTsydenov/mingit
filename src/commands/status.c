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

    printf("Changes to be committed:\n\n");

    for (int i = 0; i < index_count; i++) {
        // Защита от дубликатов в логе: показываем только последнюю (самую свежую) операцию для файла
        int is_latest = 1;
        for (int j = i + 1; j < index_count; j++) {
            if (strcmp(index_rows[i].path, index_rows[j].path) == 0) {
                is_latest = 0;
                break;
            }
        }
        if (!is_latest) continue;

        if (index_rows[i].mod == 'r') {
            printf("\033[31mDeleted:\033[0m  %s\n", index_rows[i].path);
        } else if (index_rows[i].mod == 'm') {
            printf("\033[33mModified:\033[0m %s\n", index_rows[i].path);
        } else if (index_rows[i].mod == 'a') {
            printf("\033[32mCreated:\033[0m  %s\n", index_rows[i].path);
        }
    }
    printf("\n");
    
    if (index_rows) {
        free(index_rows);
    }

    return OK;
}
