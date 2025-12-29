#include "../include/utils.h"

#include <stdlib.h>

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float clamp_float(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int wrap_coordinate(int coord, int max) {
    if (coord < 0) return max - 1;
    if (coord >= max) return 0;
    return coord;
}

float random_float() { return (float)rand() / (float)RAND_MAX; }

int random_range(int min, int max) { return min + rand() % (max - min + 1); }