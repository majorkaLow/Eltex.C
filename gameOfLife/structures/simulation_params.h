#ifndef SIMULATION_PARAMS_H
#define SIMULATION_PARAMS_H

#include <stdbool.h>

#include "../constants/constants_world.h"

typedef struct {
    int width;
    int height;

    int delay_ms;
    int max_generations;

    int max_strains;

    float initial_infection_rate;
    float initial_healthy_rate;
    float natural_death_chance;
    float vaccination_rate;
    float vaccination_effectiveness;

    int enable_mutations;
    int enable_vaccination;
    int enable_multiple_strains;
    int log_to_file;

    char stats_filename[256];

    float default_infectivity;
    float default_severity;
    float default_mutation_rate;
} SimulationParams;

#endif