#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

int clamp(int value, int min, int max);
float clamp_float(float value, float min, float max);

int wrap_coordinate(int coord, int max);

float random_float(void);
int random_range(int min, int max);

#endif