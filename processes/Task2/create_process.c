#include "task2.h"

void create_process(int process_number) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("Process%d: PID = %d, PPID = %d\n", process_number, getpid(), getppid());

        if (process_number == 1) {
            create_process(3);
            create_process(4);
        }
        else if (process_number == 2) {
            create_process(5);
        }

        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(NULL);
    }
}