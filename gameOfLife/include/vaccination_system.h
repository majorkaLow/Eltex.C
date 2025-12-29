#ifndef VACCINATION_SYSTEM_H
#define VACCINATION_SYSTEM_H

#include "../constants/constants_vaccination.h"
#include "../structures/types.h"

void init_vaccine(Vaccine* vaccine, VaccineType type, const char* name, float effectiveness,
                  float duration_min, float duration_max, float coverage, float mutation_resistance,
                  float cost, int color_pair, char symbol);
void init_vaccination_system(VaccinationSystem* vs);

float calculate_vaccine_effectiveness(const Vaccine* vaccine, const VirusStrain* strain);

void apply_vaccine_to_cell(Cell* cell, const Vaccine* vaccine, SimulationState* state);

void perform_mass_vaccination(SimulationState* state);

void process_vaccinated_cell(SimulationState* state, int y, int x);

#endif