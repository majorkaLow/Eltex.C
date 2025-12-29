#ifndef VACCINE_TYPES_H
#define VACCINE_TYPES_H

#include "virus_types.h"

typedef struct {
    VaccineType type;
    float effectiveness;
    float duration_min;
    float duration_max;
    float coverage;
    float mutation_resistance;
    float cost;
    int color_pair;
    char symbol;
    char name[32];
} Vaccine;

#endif