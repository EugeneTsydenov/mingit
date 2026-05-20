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

static int parse_log_args(int argc, char **argv, const char **commit, int *num,
                          const char **error) {
    const char *commit_value = NULL;
    int num_value = 0;

    for (int i = 2; i < argc; i++) {
        char *arg = argv[i];

        if (streq(arg, "-n")) {
            if (i + 1 >= argc) {
                *error = "log: missing value for -n";
                return 1;
            }

            if (parse_positive_int(argv[++i], &num_value)) {
                *error = "log: invalid number";
                return 1;
            }

            continue;
        }

        if (arg[0] == '-') {
            *error = "log: unknown option";
            return 1;
        }

        if (commit_value != NULL) {
            *error = "log: multiple commits specified";
            return 1;
        }

        commit_value = arg;
    }

    *commit = commit_value;
    *num = num_value;

    return 0;
}

static int parse_checkout_args(int argc, char **argv, CheckoutArgs *checkout,
                               const char **error) {
    /*
     * checkout <commit> <file>
     */

    if (argc == 4) {
        *checkout = (CheckoutArgs){
            .commit = argv[2],
            .file = argv[3],
        };

        return 0;
    }

    *error = "checkout: expected commit and file";

    return 1;
}

static ParsedCommand make_error_command(int argc, char **argv,
                                        const char *error) {
    return (ParsedCommand){
        .type = CMD_INVALID,
        .argc = argc,
        .argv = argv,
        .error = error,
    };
}

ParsedCommand parse_args(int argc, char **argv) {
    if (argc < 2) {
        return make_error_command(argc, argv, "no command provided");
    }

    char *command = argv[1];

    if (streq(command, "init")) {

        if (argc != 2) {
            return make_error_command(argc, argv, "init: too many arguments");
        }

        return (ParsedCommand){
            .type = CMD_INIT,
            .argc = argc,
            .argv = argv,
            .error = NULL,
        };
    }

    if (streq(command, "add")) {

        if (argc != 3) {
            return make_error_command(argc, argv, "add: expected path");
        }

        return (ParsedCommand){
            .type = CMD_ADD,
            .argc = argc,
            .argv = argv,
            .args.add =
                (AddArgs){
                    .file = argv[2],
                },
            .error = NULL,
        };
    }

    if (streq(command, "remove")) {

        if (argc != 3) {
            return make_error_command(argc, argv, "remove: expected path");
        }

        return (ParsedCommand){
            .type = CMD_REMOVE,
            .argc = argc,
            .argv = argv,
            .args.remove =
                (RemoveArgs){
                    .file = argv[2],
                },
            .error = NULL,
        };
    }

    if (streq(command, "commit")) {

        if (argc != 3) {
            return make_error_command(argc, argv, "commit: expected message");
        }

        return (ParsedCommand){
            .type = CMD_COMMIT,
            .argc = argc,
            .argv = argv,
            .args.commit =
                (CommitArgs){
                    .msg = argv[2],
                },
            .error = NULL,
        };
    }

    if (streq(command, "log")) {
        const char *commit = NULL;
        int num = 0;
        const char *error = NULL;

        if (parse_log_args(argc, argv, &commit, &num, &error)) {
            return make_error_command(argc, argv, error);
        }

        return (ParsedCommand){
            .type = CMD_LOG,
            .argc = argc,
            .argv = argv,
            .args.log =
                (LogArgs){
                    .commit = commit,
                    .num = num,
                },
            .error = NULL,
        };
    }

    if (streq(command, "diff")) {
        if (argc != 3) {
            return make_error_command(argc, argv, "diff: expected commit");
        }

        return (ParsedCommand){
            .type = CMD_DIFF,
            .argc = argc,
            .argv = argv,
            .args.diff =
                (DiffArgs){
                    .file = argv[2],
                },
            .error = NULL,
        };
    }

    if (streq(command, "status")) {
        if (argc != 2) {
            return make_error_command(argc, argv, "status: too many arguments");
        }

        return (ParsedCommand){
            .type = CMD_STATUS,
            .argc = argc,
            .argv = argv,
            .error = NULL,
        };
    }

    if (streq(command, "checkout")) {
        CheckoutArgs checkout = {
            .commit = NULL,
            .file = NULL,
        };
        const char *error = NULL;

        if (parse_checkout_args(argc, argv, &checkout, &error)) {
            return make_error_command(argc, argv, error);
        }

        return (ParsedCommand){
            .type = CMD_CHECKOUT,
            .argc = argc,
            .argv = argv,
            .args.checkout = checkout,
            .error = NULL,
        };
    }

    return make_error_command(argc, argv, "unknown command");
}
