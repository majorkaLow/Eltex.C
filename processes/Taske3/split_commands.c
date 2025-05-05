#include <task3.h>

int split_commands(char *input, char *cmds[], int max_cmds) {
    int num_cmds = 0;
    char *token = strtok(input, "|");
    while (token != NULL && num_cmds < max_cmds - 1) {
        cmds[num_cmds] = token;
        num_cmds++;
        token = strtok(NULL, "|");
    }
    cmds[num_cmds] = NULL;
    return num_cmds;
}