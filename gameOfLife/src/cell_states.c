#include "../include/cell_states.h"

#include <stdlib.h>
#include <string.h>

#include "../include/cell_evolution.h"
#include "../include/immunity_system.h"
#include "../include/infection_logic.h"
#include "../include/utils.h"
#include "../include/vaccination_system.h"

void process_infected_cell(SimulationState* state, int y, int x) {
    Cell* cell = &state->next_world[y][x];
    int original_strain_id = cell->strain_id;

    if (state->params.enable_mutations) {
        int new_strain = check_for_mutation(state, cell->strain_id);
        if (new_strain != cell->strain_id) {
            cell->strain_id = new_strain;
            cell->infection_stage = INF_INCUBATION;
            cell->timer = random_range(2, 4);
        }
    }

    handle_cell_evolution(state, cell, original_strain_id);

    if (cell->state == STATE_DEAD) {
        return;
    }

    if (cell->timer > 0) {
        cell->timer--;
        return;
    }

    cell->infection_stage++;

    if (cell->infection_stage == INF_SEVERE) {
        VirusStrain* strain = &state->strains[cell->strain_id];
        float immediate_death_chance = strain->severity * 0.2f;

        if (random_float() < immediate_death_chance) {
            cell->state = STATE_DEAD;
            return;
        }
    }

    if (cell->infection_stage >= INF_STAGE_COUNT) {
        VirusStrain* strain = &state->strains[cell->strain_id];
        float survival_chance = 0.6f - strain->severity * 0.3f;
        survival_chance += ((float)cell->resistance / MAX_RESISTANCE * 0.4f);
        survival_chance = clamp_float(survival_chance, MIN_SURVIVAL_CHANCE, 0.95f);

        if (random_float() < survival_chance) {
            cell->state = STATE_IMMUNE;
            cell->immunity_stage = IMM_FRESH;
            cell->timer = random_range(8, 15);
            set_immunity_to_strain(cell, cell->strain_id);

            if (cell->infection_stage == INF_SEVERE && cell->resistance < MAX_RESISTANCE) {
                cell->resistance++;
            }
        } else {
            cell->state = STATE_DEAD;
        }
    } else {
        static const int stage_durations[INF_STAGE_COUNT] = {3, 5, 4, 6};
        int base_duration = stage_durations[cell->infection_stage];

        if (cell->infection_stage == INF_SEVERE) {
            base_duration += 2;
        }

        cell->timer = random_range(base_duration - 1, base_duration + 1);
    }
}

void process_immune_cell(Cell* cell) {
    if (cell->timer > 0) {
        cell->timer--;
        return;
    }

    cell->immunity_stage++;

    if (cell->immunity_stage >= IMM_STAGE_COUNT) {
        cell->state = STATE_HEALTHY;
        cell->resistance = rand() % (MAX_RESISTANCE + 1);
    } else {
        static const int stage_durations[IMM_STAGE_COUNT] = {15, 20, 25, 30};
        int base_duration = stage_durations[cell->immunity_stage];
        cell->timer = random_range(base_duration - 3, base_duration + 3);
    }
}

void try_reproduce_cell(SimulationState* state, int y, int x) {
    Cell* cell = &state->world[y][x];

    if (cell->state != STATE_HEALTHY && cell->state != STATE_IMMUNE && cell->state != STATE_VACCINATED) {
        return;
    }

    float reproduction_chance = REPRODUCTION_CHANCE;

    if (cell->state == STATE_IMMUNE) {
        reproduction_chance += REPRODUCTION_IMMUNITY_BONUS;
    }

    if (cell->state == STATE_VACCINATED) {
        reproduction_chance *= 0.7f;
    }

    if (cell->age < 10) {
        reproduction_chance *= 1.5f;
    }

    if (cell->age > 40) {
        reproduction_chance *= 0.5f;
    }

    if (random_float() < reproduction_chance) {
        int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        for (int i = 0; i < 4; i++) {
            int ny = wrap_coordinate(y + directions[i][0], state->params.height);
            int nx = wrap_coordinate(x + directions[i][1], state->params.width);

            Cell* neighbor = &state->world[ny][nx];

            if (neighbor->state == STATE_DEAD) {
                Cell* offspring = &state->next_world[ny][nx];

                offspring->state = STATE_HEALTHY;
                offspring->age = 0;
                offspring->resistance = cell->resistance;

                if (cell->state == STATE_INFECTED && random_float() < 0.02f) {
                    offspring->state = STATE_INFECTED;
                    offspring->strain_id = cell->strain_id;
                    offspring->infection_stage = INF_INCUBATION;
                    offspring->timer = random_range(2, 4);

                    if (random_float() < 0.1f) {
                        int active_strains[MAX_STRAINS];
                        int active_count = 0;

                        for (int s = 0; s < MAX_STRAINS; s++) {
                            if (state->strains[s].base_infectivity > 0.0f) {
                                active_strains[active_count++] = s;
                            }
                        }

                        if (active_count > 0) {
                            int new_strain = active_strains[rand() % active_count];
                            if (random_float() < 0.2f) {
                                offspring->state = STATE_DEAD;
                            } else {
                                offspring->strain_id = new_strain;
                            }
                        }
                    }
                }

                if (random_float() < IMMUNITY_INHERITANCE_CHANCE) {
                    memcpy(offspring->immunity_to_strain, cell->immunity_to_strain,
                           sizeof(offspring->immunity_to_strain));

                    if (offspring->resistance < MAX_RESISTANCE && random_float() < 0.3f) {
                        offspring->resistance++;
                    }

                    if (offspring->resistance > 0 && random_float() < 0.1f) {
                        offspring->resistance--;
                    }
                } else {
                    clear_immunity(offspring);
                    if (random_float() < 0.1f) {
                        set_immunity_to_strain(offspring, 0);
                    }
                }

                if (cell->state == STATE_VACCINATED && random_float() < 0.3f) {
                    offspring->state = STATE_VACCINATED;
                    offspring->vaccine_timer = cell->vaccine_timer / 2;
                    offspring->vaccine_type = cell->vaccine_type;
                    state->vaccination.current_vaccinated++;
                }

                if (random_float() < 0.05f) {
                    if (random_float() < 0.5f && offspring->resistance < MAX_RESISTANCE) {
                        offspring->resistance++;
                    } else if (offspring->resistance > 0) {
                        offspring->resistance--;
                    }

                    if (random_float() < 0.2f) {
                        int random_strain = rand() % MAX_STRAINS;
                        if (state->strains[random_strain].base_infectivity > 0.0f) {
                            set_immunity_to_strain(offspring, random_strain);
                        }
                    }
                }
                offspring->age = 0;

                break;
            }
        }
    }
}

void process_healthy_cell(SimulationState* state, int y, int x) {
    Cell* current = &state->world[y][x];
    Cell* next = &state->next_world[y][x];

    *next = *current;
    next->age = current->age + 1;

    if (current->age > 50 && random_float() < 0.001f) {
        next->state = STATE_DEAD;
        if (current->state == STATE_VACCINATED) {
            state->vaccination.current_vaccinated--;
        }
        return;
    }
    if (next->state == STATE_VACCINATED) {
        process_vaccinated_cell(state, y, x);
    } else if (next->state == STATE_HEALTHY && state->params.enable_vaccination) {
        if (random_float() < state->vaccination.spontaneous_rate) {
            apply_vaccine_to_cell(next, &state->vaccination.current_vaccine, state);
            state->vaccination.current_vaccinated++;
        }
    }

    if (next->state == STATE_HEALTHY || next->state == STATE_IMMUNE) {
        try_reproduce_cell(state, y, x);
    }
}

void process_dead_cell(Cell* cell) {
    if (random_float() < DEAD_REVIVAL_RATE) {
        cell->state = STATE_HEALTHY;
        cell->resistance = rand() % (MAX_RESISTANCE + 1);
        cell->age = 0;
    }
}