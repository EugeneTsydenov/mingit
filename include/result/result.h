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
    REPO_ALREADY_EXISTS,
    REPO_NOT_FOUND,

    // file status
    FILE_NOT_FOUND,
    RESERVED_MINIGIT_PATH,

    NOTHING_TO_COMMIT
} Result;

static inline const char *result_message(Result result) {
    switch (result) {
    case OK:
        return "ok";
    case NO_COMMAND_PROVIDED:
        return "no command provided";
    case INIT_TOO_MANY_ARGUMENTS:
        return "init: too many arguments";
    case ADD_EXPECTED_PATH:
        return "add: expected path";
    case REMOVE_EXPECTED_PATH:
        return "remove: expected path";
    case COMMIT_EXPECTED_MESSAGE:
        return "commit: expected message";
    case LOG_MISSING_VALUE_N:
        return "log: missing value for -n";
    case LOG_INVALID_NUMBER:
        return "log: invalid number";
    case LOG_UNKNOWN_OPTION:
        return "log: unknown option";
    case LOG_MULTIPLE_COMMITS_SPECIFIED:
        return "log: multiple commits specified";
    case DIFF_EXPECTED_COMMIT:
        return "diff: expected commit";
    case STATUS_TOO_MANY_ARGUMENTS:
        return "status: too many arguments";
    case CHECKOUT_EXPECTED_COMMIT_AND_FILE:
        return "checkout: expected commit and file";
    case UNKNOWN_COMMAND:
        return "unknown command";
    case REPO_ALREADY_EXISTS:
        return "repository already exists";
    case REPO_NOT_FOUND:
        return "repository not found";
    case FILE_NOT_FOUND:
        return "file not found";
    case RESERVED_MINIGIT_PATH:
        return "cannot operate on reserved path .minigit";
    case NOTHING_TO_COMMIT:
        return "nothing to commit";
    default:
        return "unknown error";
    }
}

#endif
