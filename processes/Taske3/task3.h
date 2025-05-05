#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_CMDS 16

void execute_command(char *cmd);
void close_all_pipes(int pipefds[], int num_pipes);
void redirect_io(int i, int num_cmds, int pipefds[]);
int setup_pipes(int pipefds[], int num_pipes);
int split_commands(char *input, char *cmds[], int max_cmds);