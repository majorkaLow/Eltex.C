#include <stdio.h>
#include <stdlib.h>

#include "../data_libs/data_io.h"
#include "data_process.h"

#define ERROR_MSG "ERROR"

int main() {
    double* data = NULL;
    int n = 0;

    input(&data, &n);

    if (data == NULL || n <= 0) {
        printf("%s\n", ERROR_MSG);
        return 1;
    }

    if (normalization(data, n)) {
        output(data, n);
        printf("\n");
    } else {
        printf("%s\n", ERROR_MSG);
    }

    free(data);
    return 0;
}