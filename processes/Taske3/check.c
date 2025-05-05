#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Использование: %s <число>\n", argv[0]);
        return 1;
    }

    int number = atoi(argv[1]);
    int cube = number * number * number;

    printf("Число %d в кубе равен %d\n", number, cube);

    return 0;
}