#include "commands/init.h"
#include "repo/repo.h"
#include "result/result.h"
#include "util/util.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

Result init(void) {
    if (repo_exists_local()) {
        return REPO_ALREADY_EXISTS;
    }

    mkdir_p(MINIGIT_DIR);

    const char *file_names[] = {"HEAD", "index"};
    size_t file_count = sizeof(file_names) / sizeof(file_names[0]);

    for (size_t i = 0; i < file_count; i++) {
        repo_create_file(file_names[i]);
    }

    const char *parent = "";
    const time_t ts = time(NULL);
    const char *msg = "initial commit";

    const uint64_t h = hash_commit(parent, ts, msg, "");

    char hash_str[17];
    hash_to_string(h, hash_str);

    repo_create_commit(hash_str);
    repo_write_meta(hash_str, parent, msg, ts);
    repo_write_head(hash_str);

    printf("initialized empty minigit repository.\n");
    return OK;
}
