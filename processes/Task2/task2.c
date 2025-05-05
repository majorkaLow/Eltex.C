#include "task2.h"

int main() {
    printf("Main Process: PID = %d, PPID = %d\n", getpid(), getppid());

    create_process(1);
    create_process(2);

    return 0;
}