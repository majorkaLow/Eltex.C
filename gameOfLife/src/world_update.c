#include "../include/world_update.h"

#include <string.h>

#include "../include/cell_states.h"
#include "../include/infection_logic.h"
#include "../include/utils.h"
#include "../include/vaccination_system.h"
void update_world(SimulationState* state) {
    if (state->params.enable_vaccination && state->vaccination.enable_mass_vaccination) {
        perform_mass_vaccination(state);
    }

    copy_world_state(state);

    for (int y = 0; y < state->params.height; y++) {
        for (int x = 0; x < state->params.width; x++) {
            Cell* current = &state->world[y][x];
            Cell* next = &state->next_world[y][x];

            switch (current->state) {
                case STATE_DEAD:
                    process_dead_cell(next);
                    break;

                case STATE_HEALTHY:
                case STATE_VACCINATED:
                    process_healthy_cell(state, y, x);
                    break;

                case STATE_INFECTED:
                    process_infected_cell(state, y, x);
                    break;

                case STATE_IMMUNE:
                    process_immune_cell(next);
                    if (next->state == STATE_IMMUNE) {
                        try_reproduce_cell(state, y, x);
                    }
                    break;
            }
        }
    }

    for (int y = 0; y < state->params.height; y++) {
        for (int x = 0; x < state->params.width; x++) {
            if (state->world[y][x].state == STATE_INFECTED) {
                for (int dy = -NEIGHBORHOOD_RADIUS; dy <= NEIGHBORHOOD_RADIUS; dy++) {
                    for (int dx = -NEIGHBORHOOD_RADIUS; dx <= NEIGHBORHOOD_RADIUS; dx++) {
                        if (dx == 0 && dy == 0) continue;

                        int ny = wrap_coordinate(y + dy, state->params.height);
                        int nx = wrap_coordinate(x + dx, state->params.width);

                        Cell* target = &state->world[ny][nx];
                        Cell* next_target = &state->next_world[ny][nx];

                        if (target->state == STATE_HEALTHY && random_float() < 0.3f) {  // 30% вместо 80%
                            next_target->state = STATE_INFECTED;
                            next_target->strain_id = state->world[y][x].strain_id;
                            next_target->infection_stage = INF_INCUBATION;
                            next_target->timer = random_range(2, 4);
                            next_target->age = target->age;
                            next_target->resistance = target->resistance;
                        }
                    }
                }
            }
        }
    }

    apply_next_world(state);
    state->generation++;
}

void copy_world_state(SimulationState* state) {
    for (int y = 0; y < state->params.height; y++) {
        for (int x = 0; x < state->params.width; x++) {
            state->next_world[y][x] = state->world[y][x];
        }
    }
}

void apply_next_world(SimulationState* state) {
    for (int y = 0; y < state->params.height; y++) {
        memcpy(state->world[y], state->next_world[y], state->params.width * sizeof(Cell));
    }
}