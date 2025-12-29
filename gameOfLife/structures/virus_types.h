#ifndef VIRUS_TYPES_H
#define VIRUS_TYPES_H

#include <stdbool.h>

typedef enum {
    VACCINE_STANDARD = 0,
    VACCINE_BROAD_SPECTRUM,
    VACCINE_BOOSTER,
    VACCINE_ADAPTIVE,
    VACCINE_TYPE_COUNT
} VaccineType;

typedef struct {
    int id;
    float base_infectivity;
    float severity;
    float mutation_rate;
    int color_pair;
    char symbol;
    int resistance_penalty;
} VirusStrain;

#endif