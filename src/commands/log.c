#include "commands/log.h"

#include "repo/repo.h"
#include "result/result.h"
#include <stdio.h>
#include <string.h>

void print_commit(CommitMeta commit) {
    printf("commit %s\n", commit.hash);

    char date_str[64];
    struct tm *tm_info = localtime(&commit.timestamp);
    strftime(date_str, sizeof(date_str), "%a %b %d %H:%M:%S %Y", tm_info);
    printf("Date:   %s\n", date_str);

    printf("\n    %s\n\n", commit.msg);
}

void log_with_n(CommitMeta root, int n) {
    CommitMeta curr = root;
    for (int i = 0; i < n; i++) {
        print_commit(curr);
        if (strcmp(curr.parent, "") == 0) {
            break;
        }

        repo_read_meta(curr.parent, &curr);
    }
}

Result mlog(const char *commit, int n) {
    if (!repo_exists_local()) {
        return REPO_NOT_FOUND;
    }

    if (commit == NULL) {
        char headHash[17];
        repo_read_head(headHash);
        commit = headHash;
    }

    CommitMeta root;
    if (!repo_read_meta(commit, &root)) {
        return COMMIT_NOT_FOUND;
    }

    if (n > 0) {
        log_with_n(root, n);
        return OK;
    }

    CommitMeta curr = root;
    while (1) {
        print_commit(curr);
        if (strcmp(curr.parent, "") == 0) {
            break;
        }

        repo_read_meta(curr.parent, &curr);
    }

    return OK;
}
