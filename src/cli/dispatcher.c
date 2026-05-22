#include "cli/dispatcher.h"
#include "cli/parser.h"
#include "commands/add.h"
#include "commands/init.h"
#include "result/result.h"
#include <stdio.h>

Result dispatch_command(ParsedCommand *cmd) {
    switch (cmd->type) {
    case CMD_INIT:
        return init();
    case CMD_ADD:
        return add(cmd->args.add.file);
    case CMD_REMOVE:
        return remove(cmd->args.remove.file);
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
    }

    return OK;
}
