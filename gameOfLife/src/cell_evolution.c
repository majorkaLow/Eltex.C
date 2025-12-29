#include "../include/cell_evolution.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/utils.h"

void handle_cell_evolution(SimulationState* state, Cell* cell, int current_strain_id) {
    float evolution_chance = 0.0f;

    switch (cell->infection_stage) {
        case INF_INCUBATION:
            evolution_chance = 0.05f;
            break;
        case INF_MILD:
            evolution_chance = 0.1f;
            break;
        case INF_SEVERE:
            evolution_chance = 0.15f;
            break;
        case INF_RECOVERING:
            evolution_chance = 0.02f;
            break;
    }

    if (state->params.enable_mutations) {
        VirusStrain* strain = &state->strains[current_strain_id];
        evolution_chance *= (1.0f + strain->mutation_rate * 5.0f);
    }

    if (random_float() < evolution_chance) {
        int active_strains[MAX_STRAINS];
        int active_count = 0;

        for (int i = 0; i < MAX_STRAINS; i++) {
            if (state->strains[i].base_infectivity > 0.0f && i != current_strain_id) {
                if (state->strain_stats[i] > 0 || random_float() < 0.3f) {
                    active_strains[active_count++] = i;
                }
            }
        }

        if (active_count > 0) {
            int total_infections = 0;
            for (int i = 0; i < active_count; i++) {
                total_infections += state->strain_stats[active_strains[i]];
            }

            int selected_strain = current_strain_id;
            if (total_infections > 0) {
                int r = rand() % total_infections;
                int cumulative = 0;

                for (int i = 0; i < active_count; i++) {
                    cumulative += state->strain_stats[active_strains[i]];
                    if (r < cumulative) {
                        selected_strain = active_strains[i];
                        break;
                    }
                }
            } else {
                selected_strain = active_strains[rand() % active_count];
            }

            VirusStrain* old_strain = &state->strains[current_strain_id];
            VirusStrain* new_strain = &state->strains[selected_strain];

            float death_risk = 0.0f;
            float infectivity_diff = fabsf(old_strain->base_infectivity - new_strain->base_infectivity);
            float severity_diff = fabsf(old_strain->severity - new_strain->severity);

            death_risk += infectivity_diff * 0.4f;
            death_risk += severity_diff * 0.6f;

            death_risk = clamp_float(death_risk, 0.1f, 0.8f);

            if (random_float() < death_risk) {
                cell->state = STATE_DEAD;
                return;
            }

            cell->strain_id = selected_strain;
            cell->infection_stage = INF_INCUBATION;
            cell->timer = random_range(1, 3);

            if (state->generation % 100 == 0) {
                printf("EVOLUTION: Cell evolved from strain %c to %c\n", 'A' + current_strain_id,
                       'A' + selected_strain);
            }
        }
    }
}

void handle_reproduction_evolution(SimulationState* state, Cell* parent, Cell* offspring) {
    if (parent->state == STATE_INFECTED && random_float() < 0.1f) {
        handle_cell_evolution(state, offspring, parent->strain_id);
    }
}

int create_mutated_strain(VirusStrain strains[], int parent_strain_id) {
    for (int i = 0; i < MAX_STRAINS; i++) {
        if (strains[i].base_infectivity == 0.0f) {
            VirusStrain* parent = &strains[parent_strain_id];
            VirusStrain* new_strain = &strains[i];

            new_strain->id = i;
            new_strain->base_infectivity = parent->base_infectivity * (0.8f + random_float() * 0.4f);
            new_strain->severity = parent->severity * (0.7f + random_float() * 0.6f);
            new_strain->mutation_rate = parent->mutation_rate * (0.9f + random_float() * 0.2f);

            int penalty_change = random_float() > 0.5f ? 1 : -1;
            new_strain->resistance_penalty =
                clamp(parent->resistance_penalty + penalty_change, 0, MAX_RESISTANCE);

            new_strain->color_pair = 2 + (i % 6);
            new_strain->symbol = 'A' + i;

            return i;
        }
    }
    return parent_strain_id;
}

int check_for_mutation(SimulationState* state, int current_strain_id) {
    if (!state->params.enable_mutations) return current_strain_id;

    VirusStrain* strain = &state->strains[current_strain_id];
    if (random_float() < strain->mutation_rate) {
        return create_mutated_strain(state->strains, current_strain_id);
    }

    return current_strain_id;
}