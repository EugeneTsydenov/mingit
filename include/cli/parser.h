#ifndef PARSER_H
#define PARSER_H

#include "result/result.h"

typedef enum {
    CMD_INIT,
    CMD_ADD,
    CMD_REMOVE,
    CMD_COMMIT,
    CMD_LOG,
    CMD_DIFF,
    CMD_STATUS,
    CMD_CHECKOUT
} CommandType;

typedef struct {
    char *file;
} AddArgs;

typedef struct {
    const char *file;
} RemoveArgs;

typedef struct {
    const char *msg;
} CommitArgs;

typedef struct {
    const char *commit;
    int num;
} LogArgs;

typedef struct {
    const char *file;
} DiffArgs;

typedef struct {
    const char *commit;
    const char *file;
} CheckoutArgs;

typedef struct {
    CommandType type;

    int argc;
    char** argv;

    union {
        AddArgs add;
        RemoveArgs remove;
        CommitArgs commit;
        LogArgs log;
        DiffArgs diff;
        CheckoutArgs checkout;
    } args;
} ParsedCommand;

Result parse_args(int argc, char **argv, ParsedCommand *cmd);

#endif
