#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../data_libs/data_io.h"
#include "../data_libs/data_stat.h"
#include "decision.h"

int main() {
    double* data = NULL;
    int n = 0;

    input(&data, &n);

    if (data == NULL || n <= 0) {
        printf("NO\n");
        return 1;
    }

    printf("%s\n", make_decision(data, n) ? "YES" : "NO");

    free(data);
    return 0;
}