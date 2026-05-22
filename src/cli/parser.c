#include "cli/parser.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int streq(const char *a, const char *b) { return strcmp(a, b) == 0; }

static int parse_positive_int(const char *value, int *out) {
    char *end;

    long n = strtol(value, &end, 10);

    if (*end != '\0') {
        return 1;
    }

    if (n < 0 || n > INT_MAX) {
        return 1;
    }

    *out = (int)n;

    return 0;
}

static Result parse_log_args(int argc, char **argv, const char **commit,
                             int *num) {
    const char *commit_value = NULL;
    int num_value = 0;

    for (int i = 2; i < argc; i++) {
        char *arg = argv[i];

        if (streq(arg, "-n")) {
            if (i + 1 >= argc) {
                return LOG_MISSING_VALUE_N;
            }

            if (parse_positive_int(argv[++i], &num_value)) {
                return LOG_INVALID_NUMBER;
            }

            continue;
        }

        if (arg[0] == '-') {
            return LOG_UNKNOWN_OPTION;
        }

        if (commit_value != NULL) {
            return LOG_MULTIPLE_COMMITS_SPECIFIED;
        }

        commit_value = arg;
    }

    *commit = commit_value;
    *num = num_value;

    return OK;
}

static Result parse_checkout_args(int argc, char **argv, CheckoutArgs *checkout) {
    /*
     * checkout <commit> <file>
     */

    if (argc == 4) {
        checkout->commit = argv[2];
        checkout->file = argv[3];

        return OK;
    }

    return CHECKOUT_EXPECTED_COMMIT_AND_FILE;
}

Result parse_args(int argc, char **argv, ParsedCommand *cmd) {
    cmd->argc = argc;
    cmd->argv = argv;

    if (argc < 2) {
        return NO_COMMAND_PROVIDED;
    }

    char *command = argv[1];

    if (streq(command, "init")) {

        if (argc != 2) {
            return INIT_TOO_MANY_ARGUMENTS;
        }

        cmd->type = CMD_INIT;

        return OK;
    }

    if (streq(command, "add")) {

        if (argc != 3) {
            return ADD_EXPECTED_PATH;
        }

        cmd->type = CMD_ADD;
        cmd->args.add.file = argv[2];

        return OK;
    }

    if (streq(command, "remove")) {

        if (argc != 3) {
            return REMOVE_EXPECTED_PATH;
        }

        cmd->type = CMD_REMOVE;
        cmd->args.remove.file = argv[2];

        return OK;
    }

    if (streq(command, "commit")) {
        if (argc != 3) {
            return COMMIT_EXPECTED_MESSAGE;
        }

        cmd->type = CMD_COMMIT;
        cmd->args.commit.msg = argv[2];

        return OK;
    }

    if (streq(command, "log")) {
        const char *commit = NULL;
        int num = 0;
        Result result = parse_log_args(argc, argv, &commit, &num);

        if (result != OK) {
            return result;
        }

        cmd->type = CMD_LOG;
        cmd->args.log.commit = commit;
        cmd->args.log.num = num;

        return OK;
    }

    if (streq(command, "diff")) {
        if (argc != 3) {
            return DIFF_EXPECTED_COMMIT;
        }

        cmd->type = CMD_DIFF;
        cmd->args.diff.file = argv[2];

        return OK;
    }

    if (streq(command, "status")) {
        if (argc != 2) {
            return STATUS_TOO_MANY_ARGUMENTS;
        }

        cmd->type = CMD_STATUS;

        return OK;
    }

    if (streq(command, "checkout")) {
        CheckoutArgs checkout = {
            .commit = NULL,
            .file = NULL,
        };
        Result result = parse_checkout_args(argc, argv, &checkout);

        if (result != OK) {
            return result;
        }

        cmd->type = CMD_CHECKOUT;
        cmd->args.checkout = checkout;

        return OK;
    }

    return UNKNOWN_COMMAND;
}
