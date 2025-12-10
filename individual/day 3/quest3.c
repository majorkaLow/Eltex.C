#include <stdio.h>

// Нахождение n-ного элемента в последовательности Фибоначчи
int fibonachi(int n);

int main() {
    float x;
    if (scanf("%f", &x)) {
        if ((x == ((int)x)) && (x > 3)) {
            printf("%d\n", fibonachi((int)x));
        } else
            printf("n/a\n");
    } else
        printf("n/a\n");
    return 0;
}

/* Функции */

int fibonachi(int n) {
    int element = 1;
    int nextelement = 1;
    int x;
    for (int i = 4; i <= n; i++) {
        x = element;
        element = nextelement;
        nextelement = x + element;
    }
    return element + nextelement;
}