#include <task3.h>

int setup_pipes(int pipefds[], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe failed");
            return -1;
        }
    }
    return 0;
}