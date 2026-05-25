#include "commands/commit.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"

#include <stdlib.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int update_tracked_file(TrackedFile **files_ptr, int *count, int *capacity,
                        const char *path) {
    TrackedFile *files = *files_ptr;

    for (int i = 0; i < *count; i++) {
        if (strcmp(files[i].path, path) != 0)
            continue;

        strcpy(files[i].hash, "NEW_HASH");
        return 1;
    }

    if (*count >= *capacity) {
        *capacity = (*capacity == 0) ? 10 : (*capacity * 2);
        TrackedFile *new_files =
            realloc(files, *capacity * sizeof(TrackedFile));
        *files_ptr = new_files;
        files = new_files;
    }

    strncpy(files[*count].path, path, sizeof(files[*count].path) - 1);
    strcpy(files[*count].hash, "NEW_HASH");
    (*count)++;

    return 1;
}

void remove_tracked_file(TrackedFile *files, int *count, const char *path) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(files[i].path, path) == 0) {
            for (int j = i; j < *count - 1; j++) {
                files[j] = files[j + 1];
            }
            (*count)--;
            return;
        }
    }
}

char *tracked_files_to_string(TrackedFile *files, int count) {
    char *result = malloc(count * 512);
    result[0] = '\0';
    for (int i = 0; i < count; i++) {
        strcat(result, files[i].path);
        strcat(result, "\n");
    }
    return result;
}

Result commit(const char *msg) {
    if (!repo_exists_local() || !repo_head_exists()) {
        return REPO_NOT_FOUND;
    }

    if (!repo_index_exists()) {
        return NOTHING_TO_COMMIT;
    }

    char hash[17];
    repo_read_head(hash);

    int files_count;
    int capacity;
    TrackedFile *files = repo_read_tracked_files(hash, &files_count, &capacity);

    int rows_count;
    IndexRow *rows = repo_read_index(&rows_count);

    for (int i = 0; i < rows_count; i++) {
        if (rows[i].mod == 'r') {
            remove_tracked_file(files, &files_count, rows[i].path);
        } else {
            update_tracked_file(&files, &files_count, &capacity, rows[i].path);
        }
    }

    char *files_str = tracked_files_to_string(files, files_count);

    time_t timestamp = time(NULL);

    uint64_t newHash = hash_commit(hash, timestamp, msg, files_str);
    char hash_str[17];
    hash_to_string(newHash, hash_str);

    for (int i = 0; i < files_count; i++) {
        if (strcmp(files[i].hash, "NEW_HASH") == 0) {
            repo_write_copy_files(hash_str, files[i].path);
            strcpy(files[i].hash, hash_str);
        }
    }

    repo_create_commit(hash_str);

    repo_write_head(hash_str);
    repo_write_meta(hash_str, hash, msg, timestamp);
    repo_write_tracked_files(hash_str, files, files_count);
    repo_create_file("index");

    free(files);
    free(rows);
    free(files_str);

    return OK;
}
