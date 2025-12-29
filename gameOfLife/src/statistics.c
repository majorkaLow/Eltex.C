#include "../include/statistics.h"

#include <stdio.h>
#include <string.h>

void update_statistics(SimulationState* state) {
    memset(state->stats, 0, sizeof(state->stats));
    memset(state->infection_stats, 0, sizeof(state->infection_stats));
    memset(state->immunity_stats, 0, sizeof(state->immunity_stats));
    memset(state->strain_stats, 0, sizeof(state->strain_stats));

    for (int y = 0; y < state->params.height; y++) {
        for (int x = 0; x < state->params.width; x++) {
            Cell* cell = &state->world[y][x];
            state->stats[cell->state]++;

            if (cell->state == STATE_INFECTED) {
                state->infection_stats[cell->infection_stage]++;
                if (cell->strain_id < MAX_STRAINS) {
                    state->strain_stats[cell->strain_id]++;
                }
            } else if (cell->state == STATE_IMMUNE) {
                state->immunity_stats[cell->immunity_stage]++;
            }
        }
    }

    if (state->params.log_to_file && state->stats_file && state->generation % 10 == 0) {
        fprintf(state->stats_file, "%d", state->generation);
        for (int i = 0; i < STATE_COUNT; i++) {
            fprintf(state->stats_file, ",%d", state->stats[i]);
        }
        for (int i = 0; i < MAX_STRAINS; i++) {
            fprintf(state->stats_file, ",%d", state->strain_stats[i]);
        }
        fprintf(state->stats_file, ",%d\n", state->vaccination.current_vaccinated);
    }
}