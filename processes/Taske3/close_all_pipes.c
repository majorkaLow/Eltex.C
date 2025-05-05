#include <task3.h>

void close_all_pipes(int pipefds[], int num_pipes) {
    for (int i = 0; i < 2 * num_pipes; i++) {
        close(pipefds[i]);
    }
}