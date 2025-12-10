#include <stdio.h>

int main() {
    float x, y;

    if (scanf("%f", &x) && scanf("%f", &y)) {
        float res = x * x + y * y;

        if (res <= 25)

            printf("GOTCHA");

        else

            printf("MISS");

    } else

        printf("n/a");

    return 0;
}