#ifndef INFECTION_LOGIC_H
#define INFECTION_LOGIC_H

#include <stdbool.h>

#include "../constants/constants_infection.h"
#include "../constants/constants_vaccination.h"
#include "../structures/types.h"

float calculate_infection_modifier(int infection_stage);
float calculate_resistance_factor(int cell_resistance, int strain_resistance_penalty);
float calculate_distance_factor(int dx, int dy);
float calculate_infection_probability(const Cell* infected, const Cell* target, int dx, int dy,
                                      const VirusStrain* strain, const SimulationParams* params);
float calculate_severe_infection_mortality(const VirusStrain* strain, int cell_resistance);

bool can_infect_cell(const Cell* infected, const Cell* target);

void try_infect_cell(SimulationState* state, int infected_y, int infected_x, int target_y, int target_x,
                     int dx, int dy);
void infect_cell(SimulationState* state, int y, int x, int strain_id, const Cell* original);
void spread_infection_from_cell(SimulationState* state, int y, int x);

#endif