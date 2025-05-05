#include <task1.h>

void *print_index(void *arg)
{
    int *i = (int *)arg;
    printf("%d\n", *i);
    return NULL;
}