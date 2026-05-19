#include "cli/parser.h"
#include <stdio.h>

void print_help() {
    printf("MiniGit - simplified git implementation in C\n"
           "\n"
           "USAGE:\n"
           "    mygit <command> [options]\n"
           "\n"
           "COMMANDS:\n"
           "\n"
           "    init\n"
           "        Initialize empty repository in current directory.\n"
           "\n"
           "    add <path>\n"
           "        Add file or directory to staging area.\n"
           "        Supports recursive directory traversal.\n"
           "\n"
           "    remove <file>\n"
           "        Mark file for deletion in next commit.\n"
           "\n"
           "    commit <message>\n"
           "        Create new commit from staged changes.\n"
           "\n"
           "    status\n"
           "        Show:\n"
           "            - staged changes\n"
           "            - unstaged changes\n"
           "            - untracked files\n"
           "\n"
           "    log\n"
           "        Show commit history from current HEAD.\n"
           "\n"
           "    log -n <count>\n"
           "        Show last <count> commits.\n"
           "\n"
           "    log <commit>\n"
           "        Show history starting from specified commit or branch.\n"
           "\n"
           "    log <commit1>..<commit2>\n"
           "        Show commits from commit2 back to commit1.\n"
           "\n"
           "    diff <commit>\n"
           "        Compare current HEAD with specified commit.\n"
           "\n"
           "    checkout <commit>\n"
           "        Restore repository state from commit.\n"
           "        Activates detached HEAD mode if commit hash is used.\n"
           "\n"
           "    checkout <commit> <file>\n"
           "        Restore single file from commit.\n"
           "\n"
           "    checkout <branch>\n"
           "        Switch to branch.\n"
           "\n"
           "    branch\n"
           "        List all branches.\n"
           "\n"
           "    branch <name>\n"
           "        Create new branch from current HEAD.\n");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    return parse_args(argc, argv);
}
