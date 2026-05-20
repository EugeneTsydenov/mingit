#ifndef PARSER_H
#define PARSER_H

typedef enum {
    CMD_INIT,
    CMD_ADD,
    CMD_REMOVE,
    CMD_COMMIT,
    CMD_LOG,
    CMD_DIFF,
    CMD_STATUS,
    CMD_CHECKOUT,
    CMD_UNKNOWN,
    CMD_INVALID
} CommandType;

typedef struct {
    const char *file;
} AddArgs;

typedef struct {
    const char *file;
} RemoveArgs;

typedef struct {
    const char *msg;
} CommitArgs;

typedef struct {
    const char *commit;
    const int num;
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

    const char* error;
} ParsedCommand;

ParsedCommand parse_args(int argc, char **argv);

#endif
