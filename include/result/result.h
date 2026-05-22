#ifndef RESULT_H
#define RESULT_H

typedef enum {
    OK,

    // parse status
    NO_COMMAND_PROVIDED,
    INIT_TOO_MANY_ARGUMENTS,
    ADD_EXPECTED_PATH,
    REMOVE_EXPECTED_PATH,
    COMMIT_EXPECTED_MESSAGE,
    LOG_MISSING_VALUE_N,
    LOG_INVALID_NUMBER,
    LOG_UNKNOWN_OPTION,
    LOG_MULTIPLE_COMMITS_SPECIFIED,
    DIFF_EXPECTED_COMMIT,
    STATUS_TOO_MANY_ARGUMENTS,
    CHECKOUT_EXPECTED_COMMIT_AND_FILE,
    UNKNOWN_COMMAND,

    // repo status
    REPO_ALREADY_EXISTS
} Result;

static inline const char *result_message(Result result) {
    switch (result) {
    case OK:
        return "ok\n";
    case NO_COMMAND_PROVIDED:
        return "no command provided\n";
    case INIT_TOO_MANY_ARGUMENTS:
        return "init: too many arguments\n";
    case ADD_EXPECTED_PATH:
        return "add: expected path\n";
    case REMOVE_EXPECTED_PATH:
        return "remove: expected path\n";
    case COMMIT_EXPECTED_MESSAGE:
        return "commit: expected message\n";
    case LOG_MISSING_VALUE_N:
        return "log: missing value for -n\n";
    case LOG_INVALID_NUMBER:
        return "log: invalid number\n";
    case LOG_UNKNOWN_OPTION:
        return "log: unknown option\n";
    case LOG_MULTIPLE_COMMITS_SPECIFIED:
        return "log: multiple commits specified\n";
    case DIFF_EXPECTED_COMMIT:
        return "diff: expected commit\n";
    case STATUS_TOO_MANY_ARGUMENTS:
        return "status: too many arguments\n";
    case CHECKOUT_EXPECTED_COMMIT_AND_FILE:
        return "checkout: expected commit and file\n";
    case UNKNOWN_COMMAND:
        return "unknown command\n";
    case REPO_ALREADY_EXISTS:
        return "repository already exists.\n";
    default:
        return "unknown error\n";
    }
}

#endif
