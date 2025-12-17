#include "data_process.h"

#include <math.h>
#include <stdio.h>

#include "../data_libs/data_stat.h"

int normalization(double* data, int n) {
    if (!data || n <= 0) return 0;

    const double max_value = max(data, n);
    const double min_value = min(data, n);
    const double range = max_value - min_value;

    if (fabs(range) <= EPS) return 0;

    const double inv_range = 1.0 / range;

    for (int i = 0; i < n; i++) {
        data[i] = (data[i] - min_value) * inv_range;
    }

    return 1;
}