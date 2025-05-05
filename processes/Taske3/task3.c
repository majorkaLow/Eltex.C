#include <task3.h>

int main()
{
    char input[MAX_INPUT];
    char *cmds[MAX_CMDS];
    int status;
    pid_t pid;

    while (1) {
        printf("my_shell> ");
        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        int num_cmds = split_commands(input, cmds, MAX_CMDS);
        if (num_cmds == 0) continue;

        int num_pipes = num_cmds - 1;
        int pipefds[2 * num_pipes];

        if (num_pipes > 0 && setup_pipes(pipefds, num_pipes) < 0) {
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_cmds; i++) {
            pid = fork();
            if (pid < 0) {
                perror("fork failed");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) {
                redirect_io(i, num_cmds, pipefds);
                close_all_pipes(pipefds, num_pipes);
                execute_command(cmds[i]);
            }
        }

        close_all_pipes(pipefds, num_pipes);
        for (int i = 0; i < num_cmds; i++) {
            wait(&status);
        }
    }

    return 0;
}
