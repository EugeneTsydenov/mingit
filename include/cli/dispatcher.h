#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "cli/parser.h"
#include "result/result.h"

Result dispatch_command(ParsedCommand *cmd);

#endif
