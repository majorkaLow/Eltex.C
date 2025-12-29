#include "../include/input_handler.h"

#include <ncurses.h>
#include <stdlib.h>

#include "../include/initialization.h"

void handle_user_input(SimulationState* state) {
    int ch = getch();

    switch (ch) {
        case 'p':
        case 'P':
        case ' ':
            state->paused = !state->paused;
            break;

        case 's':
        case 'S':
            state->show_stats = !state->show_stats;
            break;

        case 'v':
        case 'V':
            state->params.enable_vaccination = !state->params.enable_vaccination;
            state->vaccination.enable_vaccination = state->params.enable_vaccination;
            break;

        case 'm':
        case 'M':
            state->params.enable_mutations = !state->params.enable_mutations;
            break;

        case 'r':
        case 'R':
            reset_simulation(state);
            break;

        case 'q':
        case 'Q':
            if (state->stats_file) fclose(state->stats_file);
            free_grid(state->world, state->params.height);
            free_grid(state->next_world, state->params.height);
            endwin();
            exit(0);
            break;
    }
    handle_vaccination_input(state, ch);
}

void handle_vaccination_input(SimulationState* state, int ch) {
    VaccinationSystem* vs = &state->vaccination;

    if (!vs->enable_vaccination) return;

    switch (ch) {
        case 't':
        case 'T':
            vs->last_mass_vaccination_gen = state->generation - vs->mass_vaccination_interval;
            break;

        case '1':
            vs->current_vaccine = vs->available_vaccines[VACCINE_STANDARD];
            break;

        case '2':
            vs->current_vaccine = vs->available_vaccines[VACCINE_BROAD_SPECTRUM];
            break;

        case '3':
            vs->current_vaccine = vs->available_vaccines[VACCINE_BOOSTER];
            break;

        case '4':
            vs->current_vaccine = vs->available_vaccines[VACCINE_ADAPTIVE];
            break;

        case '+':
            vs->mass_vaccination_interval += 10;
            break;

        case '-':
            if (vs->mass_vaccination_interval > 25) {
                vs->mass_vaccination_interval -= 10;
            }
            break;

        case '[':
            if (vs->mass_vaccination_coverage > 0.1f) {
                vs->mass_vaccination_coverage -= 0.1f;
            }
            break;

        case ']':
            if (vs->mass_vaccination_coverage < 1.0f) {
                vs->mass_vaccination_coverage += 0.1f;
            }
            break;
    }
}