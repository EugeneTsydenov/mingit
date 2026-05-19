#include "cli/parser.h"
#include <stdio.h>

int parse_args(int argc, char **argv) {
    if (argc > 0) {
        printf("%s", argv[0]);
    }

    return 0;
}
