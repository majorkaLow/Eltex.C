#include "data_io.h"

#include <stdio.h>
#include <stdlib.h>

void input(double** data, int* n) {
    char temp;
    *n = 0;

    // Read size with proper error handling
    if (scanf("%d", n) != 1 || *n <= 0) {
        *n = 0;
        return;
    }

    // Clear buffer
    while ((temp = getchar()) != '\n' && temp != EOF);

    // Allocate memory
    *data = (double*)malloc(*n * sizeof(double));
    if (*data == NULL) {
        *n = 0;
        return;
    }

    // Read data with validation
    for (int i = 0; i < *n; i++) {
        if (scanf("%lf", &(*data)[i]) != 1) {
            free(*data);
            *data = NULL;
            *n = 0;
            return;
        }
    }
}

void output(double* data, int n) {
    if (!data || n <= 0) return;

    for (int i = 0; i < n; i++) {
        printf("%.2lf", data[i]);
        if (i < n - 1) printf(" ");
    }
}