#include "../include/initialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/vaccination_system.h"

void init_default_params(SimulationParams* params) {
    params->width = DEFAULT_WIDTH;
    params->height = DEFAULT_HEIGHT;
    params->delay_ms = DEFAULT_DELAY_MS;
    params->max_generations = MAX_GENERATION;
    params->max_strains = 3;

    params->initial_infection_rate = INITIAL_INFECTION_RATE;
    params->initial_healthy_rate = INITIAL_HEALTHY_RATE;
    params->natural_death_chance = NATURAL_DEATH_CHANCE;
    params->vaccination_rate = DEFAULT_VACCINATION_RATE;
    params->vaccination_effectiveness = VACCINATION_EFFECTIVENESS;

    params->enable_mutations = 1;
    params->enable_vaccination = 0;
    params->enable_multiple_strains = 1;
    params->log_to_file = 0;
    strcpy(params->stats_filename, STATS_FILE);

    params->default_infectivity = DEFAULT_INFECTIVITY;
    params->default_severity = DEFAULT_SEVERITY;
    params->default_mutation_rate = DEFAULT_MUTATION_RATE;
}

void init_strain(VirusStrain* strain, int id, float infectivity, float severity, float mutation_rate,
                 int color_pair, char symbol, int resistance_penalty) {
    strain->id = id;
    strain->base_infectivity = infectivity;
    strain->severity = severity;
    strain->mutation_rate = mutation_rate;
    strain->color_pair = color_pair;
    strain->symbol = symbol;
    strain->resistance_penalty = clamp(resistance_penalty, 0, MAX_RESISTANCE);
}

void init_strains(VirusStrain strains[], const SimulationParams* params) {
    init_strain(&strains[0], 0, params->default_infectivity, params->default_severity,
                params->default_mutation_rate, 2, 'A', 0);

    init_strain(&strains[1], 1, params->default_infectivity * 1.5f, params->default_severity * 0.8f,
                params->default_mutation_rate * 2.0f, 3, 'B', 1);

    init_strain(&strains[2], 2, params->default_infectivity * 0.8f, params->default_severity * 1.5f,
                params->default_mutation_rate * 0.5f, 4, 'C', 2);

    for (int i = 3; i < MAX_STRAINS; i++) {
        init_strain(&strains[i], i, 0.0f, 0.0f, 0.0f, 2, 'A' + i, 0);
    }
}

Cell** allocate_grid(int height, int width) {
    Cell** grid = (Cell**)malloc(height * sizeof(Cell*));
    if (!grid) return NULL;

    for (int y = 0; y < height; y++) {
        grid[y] = (Cell*)calloc(width, sizeof(Cell));
        if (!grid[y]) {
            for (int i = 0; i < y; i++) free(grid[i]);
            free(grid);
            return NULL;
        }
    }
    return grid;
}

void free_grid(Cell** grid, int height) {
    if (!grid) return;
    for (int y = 0; y < height; y++) free(grid[y]);
    free(grid);
}

void init_cell(Cell* cell, CellState state, int resistance, int age, int strain_id) {
    memset(cell, 0, sizeof(Cell));
    cell->state = state;
    cell->resistance = resistance;
    cell->age = age;
    cell->strain_id = strain_id;

    if (state == STATE_INFECTED) {
        cell->infection_stage = INF_MILD;
        cell->timer = random_range(3, 6);
    }
}

void init_world(SimulationState* state) {
    state->world = allocate_grid(state->params.height, state->params.width);
    state->next_world = allocate_grid(state->params.height, state->params.width);

    if (!state->world || !state->next_world) {
        fprintf(stderr, "Ошибка выделения памяти для мира\n");
        exit(1);
    }

    for (int y = 0; y < state->params.height; y++) {
        for (int x = 0; x < state->params.width; x++) {
            float r = random_float();
            Cell* cell = &state->world[y][x];

            if (r < state->params.initial_infection_rate) {
                init_cell(cell, STATE_INFECTED, rand() % (MAX_RESISTANCE + 1), 0, 0);
            } else if (r < state->params.initial_infection_rate + state->params.initial_healthy_rate) {
                init_cell(cell, STATE_HEALTHY, rand() % (MAX_RESISTANCE + 1), random_range(0, 20), 0);

                if (state->params.enable_vaccination && random_float() < state->params.vaccination_rate) {
                    apply_vaccine_to_cell(cell, &state->vaccination.current_vaccine, state);
                    state->vaccination.current_vaccinated++;
                }
            } else {
                init_cell(cell, STATE_DEAD, 0, 0, 0);
            }
        }
    }
}

void reset_simulation(SimulationState* state) {
    free_grid(state->world, state->params.height);
    free_grid(state->next_world, state->params.height);

    init_strains(state->strains, &state->params);
    init_vaccination_system(&state->vaccination);
    memset(state->infections_per_strain, 0, sizeof(state->infections_per_strain));
    init_world(state);

    state->generation = 0;
}