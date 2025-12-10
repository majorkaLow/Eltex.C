#include <stdio.h>

float searchmax(float x, float y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

int main() {
    float x, y;

    if (scanf("%f", &x) && scanf("%f", &y)) {
        if ((x == (int)x) && y == (int)y) {
            printf("%.0f", searchmax(x, y));
        } else
            printf("n/a");
    } else
        printf("n/a");
    return 0;
}