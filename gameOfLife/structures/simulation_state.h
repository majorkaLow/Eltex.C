#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

#include <stdio.h>

#include "cell_types.h"
#include "simulation_params.h"
#include "vaccination_system_types.h"
#include "virus_types.h"

typedef struct {
    Cell** world;
    Cell** next_world;

    VirusStrain strains[MAX_STRAINS];

    SimulationParams params;

    VaccinationSystem vaccination;

    int generation;
    int paused;
    int show_stats;
    int show_help;

    FILE* stats_file;

    int stats[STATE_COUNT];
    int infection_stats[INF_STAGE_COUNT];
    int immunity_stats[IMM_STAGE_COUNT];
    int strain_stats[MAX_STRAINS];
    int infections_per_strain[MAX_STRAINS];
} SimulationState;

#endif