#include "decision.h"

#include <math.h>
#include <stdio.h>

#include "../data_libs/data_stat.h"

int make_decision(double* data, int n) {
    if (!data || n <= 0) return 0;

    const double m = mean(data, n);
    if (m < GOLDEN_RATIO) return 0;

    const double sigma = sqrt(variance(data, n));
    const double max_value = max(data, n);
    const double min_value = min(data, n);

    const double upper_bound = m + 3 * sigma;
    const double lower_bound = m - 3 * sigma;

    return (max_value <= upper_bound) && (min_value >= lower_bound);
}