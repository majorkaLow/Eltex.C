#include <math.h>
#include <stdio.h>

int main() {
    double x;

    if (scanf("%lf", &x) != 1) {
        printf("n/a");
        return 0;
    }

    if (x == 0 || (x * x) / 2.0 == 0) {
        printf("n/a");
        return 0;
    }

    double y =
        7e-3 * pow(x, 4) + ((22.8 * cbrt(x) - 1e3) * x + 3) / (x * x / 2.0) - x * pow(10 + x, 2.0 / x) - 1.01;

    printf("%.1lf", y);

    return 0;
}