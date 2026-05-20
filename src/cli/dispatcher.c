#include "cli/parser.h"
#include <stdio.h>

int dispatch_command(ParsedCommand *cmd) {
    if (cmd->type == CMD_INVALID || cmd->type == CMD_UNKNOWN) {
        if (cmd->error != NULL) {
            fprintf(stderr, "%s\n", cmd->error);
        }

        return 1;
    }

    switch (cmd->type) {
    case CMD_INIT:
        break;
    case CMD_ADD:
        break;
    case CMD_REMOVE:
        break;
    case CMD_COMMIT:
        break;
    case CMD_LOG:
        break;
    case CMD_DIFF:
        break;
    case CMD_STATUS:
        break;
    case CMD_CHECKOUT:
        break;
    case CMD_UNKNOWN:
    case CMD_INVALID:
        break;
    default:
        fprintf(stderr, "unreachable command type\n");
        return 1;
    }

    return 0;
}
