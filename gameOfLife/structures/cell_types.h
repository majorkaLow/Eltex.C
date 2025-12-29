#ifndef CELL_TYPES_H
#define CELL_TYPES_H

#include <stdbool.h>

#include "../constants/constants_world.h"

typedef enum {
    STATE_DEAD = 0,
    STATE_HEALTHY,
    STATE_INFECTED,
    STATE_IMMUNE,
    STATE_VACCINATED,
    STATE_COUNT
} CellState;

typedef enum { INF_INCUBATION = 0, INF_MILD, INF_SEVERE, INF_RECOVERING, INF_STAGE_COUNT } InfectionStage;

typedef enum { IMM_FRESH = 0, IMM_STRONG, IMM_WANING, IMM_WEAK, IMM_STAGE_COUNT } ImmunityStage;

typedef struct {
    unsigned char state : 3;
    unsigned char infection_stage : 2;
    unsigned char immunity_stage : 2;
    unsigned char timer : 4;
    unsigned char age : 6;
    unsigned char resistance : 3;
    unsigned char strain_id : 4;
    unsigned char is_vaccinated : 1;
    unsigned char vaccine_timer : 6;
    unsigned char vaccine_type : 2;
    unsigned char immunity_to_strain[MAX_IMMUNITY_BYTES];
} Cell;

#endif