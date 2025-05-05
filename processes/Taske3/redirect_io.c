#include <task3.h>

void redirect_io(int i, int num_cmds, int pipefds[]) {
    if (i > 0) {
        if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
            perror("dup2 failed (input)");
            exit(EXIT_FAILURE);
        }
    }
    if (i < num_cmds - 1) {
        if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
            perror("dup2 failed (output)");
            exit(EXIT_FAILURE);
        }
    }
}