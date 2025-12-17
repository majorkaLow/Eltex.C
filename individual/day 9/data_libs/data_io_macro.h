#ifndef DATA_IO_MACRO_H
#define DATA_IO_MACRO_H

#include <stdio.h>
#include <stdlib.h>

// Generic input macro for any type
#define INPUT_ARRAY(type, ptr, size)                    \
    do {                                                \
        if (scanf("%d", &(size)) != 1 || (size) <= 0) { \
            (size) = 0;                                 \
            (ptr) = NULL;                               \
            break;                                      \
        }                                               \
        (ptr) = (type*)malloc((size) * sizeof(type));   \
        if (!(ptr)) {                                   \
            (size) = 0;                                 \
            break;                                      \
        }                                               \
        for (int i = 0; i < (size); i++) {              \
            if (scanf("%lf", &(ptr)[i]) != 1) {         \
                free(ptr);                              \
                (ptr) = NULL;                           \
                (size) = 0;                             \
                break;                                  \
            }                                           \
        }                                               \
    } while (0)

// Generic output macro for any type
#define OUTPUT_ARRAY(ptr, size, format)          \
    do {                                         \
        if ((ptr) && (size) > 0) {               \
            for (int i = 0; i < (size); i++) {   \
                printf(format, (ptr)[i]);        \
                if (i < (size) - 1) printf(" "); \
            }                                    \
        }                                        \
    } while (0)

// Compatibility macros for double arrays
#define input(ptr, size) INPUT_ARRAY(double, *(ptr), *(size))
#define output(ptr, size) OUTPUT_ARRAY(ptr, size, "%.2lf")

#endif  // DATA_IO_MACRO_H