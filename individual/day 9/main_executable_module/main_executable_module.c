#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef USE_DYNAMIC
#include <dlfcn.h>
#endif

// Configure IO implementation
#ifdef USE_MACRO_IO
#include "../data_libs/data_io_macro.h"
#else
#include "../data_libs/data_io.h"
#endif

#include "../data_libs/data_stat.h"
#ifndef USE_DYNAMIC
#include "../data_module/data_process.h"
#endif
#include "../yet_another_decision_module/decision.h"

#define ERROR_MSG "ERROR"
#define LIB_PATH "./data_process.so"

#ifdef USE_DYNAMIC
typedef int (*normalization_fn)(double*, int);
#endif

int main() {
    double* data = NULL;
    int n = 0;

#ifdef USE_DYNAMIC
    void* handle = dlopen(LIB_PATH, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    normalization_fn normalization = (normalization_fn)dlsym(handle, "normalization");
    if (!normalization) {
        fprintf(stderr, "%s\n", dlerror());
        dlclose(handle);
        return 1;
    }
#endif

    printf("LOAD DATA...\n");
    input(&data, &n);

    if (data == NULL || n <= 0) {
        printf("%s\n", ERROR_MSG);
#ifdef USE_DYNAMIC
        dlclose(handle);
#endif
        return 1;
    }

    printf("RAW DATA:\n\t");
    output(data, n);
    printf("\n");

    printf("NORMALIZED DATA:\n\t");
    if (!normalization(data, n)) {
        printf("%s\n", ERROR_MSG);
        free(data);
#ifdef USE_DYNAMIC
        dlclose(handle);
#endif
        return 1;
    }
    output(data, n);
    printf("\n");

    printf("SORTED NORMALIZED DATA:\n\t");
    sort(data, n);
    output(data, n);
    printf("\n");

    printf("FINAL DECISION:\n\t");
    printf("%s\n", make_decision(data, n) ? "YES" : "NO");

    free(data);
#ifdef USE_DYNAMIC
    dlclose(handle);
#endif
    return 0;
}