#include "cli/dispatcher.h"

#include "cli/parser.h"
#include "commands/add.h"
#include "commands/commit.h"
#include "commands/checkout.h"
#include "commands/diff.h"
#include "commands/init.h"
#include "commands/log.h"
#include "commands/status.h"
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
        return commit(cmd->args.commit.msg);
    case CMD_LOG:
        return mlog(cmd->args.log.commit, cmd->args.log.num);
    case CMD_DIFF:
        return diff(cmd->args.diff.target_commit);
    case CMD_STATUS:
        return status();
    case CMD_CHECKOUT:
        return checkout(cmd->args.checkout.commit, cmd->args.checkout.file);
    }

    return OK;
}
