#include "../include/infection_logic.h"

#include <math.h>
#include <string.h>

#include "../include/immunity_system.h"
#include "../include/utils.h"

float calculate_infection_modifier(int infection_stage) {
    static const float modifiers[INF_STAGE_COUNT] = {0.0f, 0.5f, 0.8f, 0.2f};
    return infection_stage < INF_STAGE_COUNT ? modifiers[infection_stage] : 0.0f;
}

float calculate_resistance_factor(int cell_resistance, int strain_resistance_penalty) {
    int effective_resistance = cell_resistance - strain_resistance_penalty;
    if (effective_resistance < 0) effective_resistance = 0;
    return 1.0f - ((float)effective_resistance / MAX_RESISTANCE * RESISTANCE_PROTECTION_FACTOR);
}

float calculate_distance_factor(int dx, int dy) {
    float distance = sqrtf(dx * dx + dy * dy);
    return 1.0f / (distance * DISTANCE_ATTENUATION);
}

float calculate_infection_probability(const Cell* infected, const Cell* target, int dx, int dy,
                                      const VirusStrain* strain, const SimulationParams* params) {
    float base_chance = strain->base_infectivity * calculate_infection_modifier(infected->infection_stage);

    float resistance_factor = calculate_resistance_factor(target->resistance, strain->resistance_penalty);
    base_chance *= resistance_factor;

    float distance_factor = calculate_distance_factor(dx, dy);
    base_chance *= distance_factor;

    if (target->state == STATE_VACCINATED) {
        float vaccine_protection = params->vaccination_effectiveness;

        float time_factor = 1.0f;
        if (target->vaccine_timer > 0) {
            time_factor = 0.5f + 0.5f * ((float)target->vaccine_timer / VACCINE_DURATION_MAX);
        }

        base_chance *= (1.0f - vaccine_protection * time_factor);
    }

    if (target->state == STATE_IMMUNE) {
        float immunity_protection = 0.3f;

        float stage_modifier = 1.0f - (float)target->immunity_stage / IMM_STAGE_COUNT * 0.5f;
        base_chance *= (1.0f - immunity_protection * stage_modifier);
    }

    return clamp_float(base_chance, 0.01f, 0.95f);
}

float calculate_severe_infection_mortality(const VirusStrain* strain, int cell_resistance) {
    float base_mortality = strain->severity * 0.5f;
    float resistance_factor = 1.0f - ((float)cell_resistance / MAX_RESISTANCE * 0.4f);
    return clamp_float(base_mortality * resistance_factor, 0.1f, 0.9f);
}  // не юзается

bool can_infect_cell(const Cell* infected, const Cell* target) {
    if (target->state == STATE_INFECTED || target->state == STATE_DEAD) {
        return false;
    }
    if (target->state == STATE_IMMUNE || target->state == STATE_VACCINATED) {
        if (has_immunity_to_strain(target, infected->strain_id)) {
            return false;
        }
    }
    return true;
}

void try_infect_cell(SimulationState* state, int infected_y, int infected_x, int target_y, int target_x,
                     int dx, int dy) {
    Cell* infected = &state->world[infected_y][infected_x];
    Cell* target = &state->world[target_y][target_x];

    if (!can_infect_cell(infected, target)) return;

    VirusStrain* strain = &state->strains[infected->strain_id];
    float chance = calculate_infection_probability(infected, target, dx, dy, strain, &state->params);

    if (random_float() < chance) {
        infect_cell(state, target_y, target_x, infected->strain_id, target);
    }
}

void infect_cell(SimulationState* state, int y, int x, int strain_id, const Cell* original) {
    Cell* next = &state->next_world[y][x];

    next->state = STATE_INFECTED;
    next->strain_id = strain_id;
    next->infection_stage = INF_INCUBATION;
    next->timer = random_range(2, 4);
    next->age = original->age;
    next->resistance = original->resistance;
    next->vaccine_timer = original->vaccine_timer;
    next->vaccine_type = original->vaccine_type;

    memcpy(next->immunity_to_strain, original->immunity_to_strain, sizeof(next->immunity_to_strain));

    state->infections_per_strain[strain_id]++;

    if (original->state == STATE_VACCINATED) {
        state->vaccination.current_vaccinated--;
    }
}

void spread_infection_from_cell(SimulationState* state, int y, int x) {
    Cell* infected = &state->world[y][x];

    if (infected->state != STATE_INFECTED) {
        return;
    }

    for (int dy = -NEIGHBORHOOD_RADIUS; dy <= NEIGHBORHOOD_RADIUS; dy++) {
        for (int dx = -NEIGHBORHOOD_RADIUS; dx <= NEIGHBORHOOD_RADIUS; dx++) {
            if (dx == 0 && dy == 0) continue;

            int ny = wrap_coordinate(y + dy, state->params.height);
            int nx = wrap_coordinate(x + dx, state->params.width);

            try_infect_cell(state, y, x, ny, nx, dx, dy);
        }
    }
}  // не юзается