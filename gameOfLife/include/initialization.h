#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "../constants/constants_infection.h"
#include "../constants/constants_primary.h"
#include "../constants/constants_vaccination.h"
#include "../structures/types.h"

void init_default_params(SimulationParams* params);

void init_strain(VirusStrain* strain, int id, float infectivity, float severity, float mutation_rate,
                 int color_pair, char symbol, int resistance_penalty);
void init_strains(VirusStrain strains[], const SimulationParams* params);

Cell** allocate_grid(int height, int width);
void free_grid(Cell** grid, int height);

void init_cell(Cell* cell, CellState state, int resistance, int age, int strain_id);
void init_world(SimulationState* state);

void reset_simulation(SimulationState* state);

#endif