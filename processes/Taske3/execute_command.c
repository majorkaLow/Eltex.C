#include <task3.h>

void execute_command(char *cmd)
{
    char *args[MAX_ARGS];
    char *token = strtok(cmd, " ");
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1)
    {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}