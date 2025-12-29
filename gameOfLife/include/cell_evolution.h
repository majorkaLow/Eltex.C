#ifndef CELL_EVOLUTION_H
#define CELL_EVOLUTION_H

#include "../constants/constants_infection.h"
#include "../structures/types.h"

void handle_cell_evolution(SimulationState* state, Cell* cell, int current_strain_id);
void handle_reproduction_evolution(SimulationState* state, Cell* parent, Cell* offspring);

int create_mutated_strain(VirusStrain strains[], int parent_strain_id);
int check_for_mutation(SimulationState* state, int current_strain_id);

#endif