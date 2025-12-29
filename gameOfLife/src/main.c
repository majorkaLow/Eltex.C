#include <getopt.h>
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../include/command_line.h"
#include "../include/display_system.h"
#include "../include/file_operations.h"
#include "../include/initialization.h"
#include "../include/input_handler.h"
#include "../include/statistics.h"
#include "../include/utils.h"
#include "../include/vaccination_system.h"
#include "../include/world_update.h"
#include "../structures/types.h"

int main(int argc, char* argv[]) {
    SimulationState state = {0};

    init_default_params(&state.params);
    init_vaccination_system(&state.vaccination);
    parse_command_line_arguments(argc, argv, &state);

    if (state.show_help) {
        print_help_message();
        return 0;
    }

    open_statistics_file(&state);

    initscr();
    curs_set(0);
    timeout(0);
    noecho();
    keypad(stdscr, TRUE);

    if (!check_terminal_size(state.params.width, state.params.height)) {
        return 1;
    }

    if (has_colors()) {
        init_display_colors();
    }

    srand(time(NULL));

    init_strains(state.strains, &state.params);
    memset(state.infections_per_strain, 0, sizeof(state.infections_per_strain));
    init_world(&state);
    update_statistics(&state);

    state.show_stats = 1;

    while (state.generation < state.params.max_generations) {
        clear();

        display_world(&state);
        if (state.show_stats) {
            display_statistics(&state);
            display_legend(&state);
        }

        if (state.params.enable_vaccination) {
            display_vaccination_stats(&state);
        }

        if (!state.paused) {
            update_world(&state);
            update_statistics(&state);
        } else {
            attron(COLOR_PAIR(11));
            mvprintw(0, 0, "PAUSED");
            attroff(COLOR_PAIR(11));
        }

        refresh();
        handle_user_input(&state);
        usleep(state.params.delay_ms * 1000);
    }
    if (state.stats_file) fclose(state.stats_file);
    free_grid(state.world, state.params.height);
    free_grid(state.next_world, state.params.height);
    endwin();

    printf("Simulation completed after %d generations.\n", state.generation);
    if (state.params.log_to_file) {
        printf("Statistics saved to: %s\n", state.params.stats_filename);
    }

    return 0;
}