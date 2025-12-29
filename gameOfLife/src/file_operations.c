#include "../include/file_operations.h"

#include <stdio.h>

void open_statistics_file(SimulationState* state) {
    if (!state->params.log_to_file) return;

    state->stats_file = fopen(state->params.stats_filename, "w");
    if (!state->stats_file) {
        state->params.log_to_file = 0;
        return;
    }

    fprintf(state->stats_file, "generation,dead,healthy,infected,immune,vaccinated");
    for (int i = 0; i < MAX_STRAINS; i++) {
        fprintf(state->stats_file, ",strain_%c", 'A' + i);
    }
    fprintf(state->stats_file, ",vaccinated_total\n");
}