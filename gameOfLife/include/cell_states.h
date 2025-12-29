#ifndef CELL_STATES_H
#define CELL_STATES_H

#include "../constants/constants_evolution.h"
#include "../constants/constants_primary.h"
#include "../structures/types.h"

void process_infected_cell(SimulationState* state, int y, int x);
void process_healthy_cell(SimulationState* state, int y, int x);
void process_immune_cell(Cell* cell);
void process_dead_cell(Cell* cell);

void try_reproduce_cell(SimulationState* state, int y, int x);

#endif