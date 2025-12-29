#include "../include/display_system.h"

#include <ncurses.h>
#include <string.h>

void init_display_colors(void) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     // Здоровый
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);    // Штамм A
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);   // Штамм B
    init_pair(4, COLOR_RED, COLOR_BLACK);       // Штамм C
    init_pair(5, COLOR_CYAN, COLOR_BLACK);      // Штамм D
    init_pair(6, COLOR_BLUE, COLOR_BLACK);      // Штамм E
    init_pair(7, COLOR_WHITE, COLOR_BLACK);     // Штамм F
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);    // Штамм G
    init_pair(9, COLOR_GREEN, COLOR_BLACK);     // Вакцинированный
    init_pair(10, COLOR_WHITE, COLOR_BLACK);    // Статистика
    init_pair(11, COLOR_RED, COLOR_BLACK);      // Заголовки
    init_pair(12, COLOR_BLUE, COLOR_BLACK);     // Иммунный
    init_pair(13, COLOR_CYAN, COLOR_BLACK);     // Дополнительные цвета
    init_pair(14, COLOR_MAGENTA, COLOR_BLACK);  // Дополнительные цвета
}

bool check_terminal_size(int width, int height) {
    int required_width = width * CELL_WIDTH + 10;
    int required_height = height + 20;

    if (LINES < required_height || COLS < required_width) {
        endwin();
        printf("Terminal too small! Minimum: %dx%d\n", required_width, required_height);
        printf("Current size: %dx%d\n", COLS, LINES);
        printf("Use -w and -h options to reduce world size.\n");
        return false;
    }
    return true;
}

int get_cell_color(const Cell* cell) {
    switch (cell->state) {
        case STATE_HEALTHY:
            return COLOR_PAIR(1);

        case STATE_INFECTED:
            if (cell->strain_id < MAX_STRAINS) {
                return COLOR_PAIR(2 + (cell->strain_id % 6));
            }
            return COLOR_PAIR(2);

        case STATE_IMMUNE:
            return COLOR_PAIR(12);

        case STATE_VACCINATED:
            return COLOR_PAIR(9);

        default:
            return COLOR_PAIR(10);
    }
}

char get_cell_character(const Cell* cell) {
    switch (cell->state) {
        case STATE_DEAD:
            return ' ';

        case STATE_HEALTHY:
            return '.';

        case STATE_INFECTED:
            if (cell->strain_id < MAX_STRAINS) {
                return 'A' + cell->strain_id;
            }
            return 'I';

        case STATE_IMMUNE:
            static const char immunity_chars[IMM_STAGE_COUNT] = {'F', 'S', 'W', 'w'};
            return immunity_chars[cell->immunity_stage];

        case STATE_VACCINATED:
            return 'V';
    }
    return '?';
}

void display_world(const SimulationState* state) {
    for (int y = 0; y < state->params.height; y++) {
        for (int x = 0; x < state->params.width; x++) {
            const Cell* cell = &state->world[y][x];
            int color = get_cell_color(cell);
            char ch = get_cell_character(cell);

            attron(color);
            mvaddch(y, x * CELL_WIDTH, ch);
            if (CELL_WIDTH > 1) {
                mvaddch(y, x * CELL_WIDTH + 1, ' ');
            }
            attroff(color);
        }
    }
}

void display_statistics(const SimulationState* state) {
    int total_cells = state->params.width * state->params.height;

    attron(COLOR_PAIR(11));
    mvprintw(state->params.height + 2, 0, "=== EPIDEMIC SIMULATION ===");
    attroff(COLOR_PAIR(11));

    attron(COLOR_PAIR(10));
    mvprintw(state->params.height + 3, 0, "Generation: %d", state->generation);
    mvprintw(state->params.height + 4, 0, "Dead: %d (%.1f%%)", state->stats[STATE_DEAD],
             (float)state->stats[STATE_DEAD] / total_cells * 100);
    mvprintw(state->params.height + 5, 0, "Healthy: %d (%.1f%%)", state->stats[STATE_HEALTHY],
             (float)state->stats[STATE_HEALTHY] / total_cells * 100);
    mvprintw(state->params.height + 6, 0, "Infected: %d (%.1f%%)", state->stats[STATE_INFECTED],
             (float)state->stats[STATE_INFECTED] / total_cells * 100);
    mvprintw(state->params.height + 7, 0, "Immune: %d (%.1f%%)", state->stats[STATE_IMMUNE],
             (float)state->stats[STATE_IMMUNE] / total_cells * 100);
    mvprintw(state->params.height + 8, 0, "Vaccinated: %d (%.1f%%)", state->vaccination.current_vaccinated,
             (float)state->vaccination.current_vaccinated / total_cells * 100);

    if (state->params.enable_multiple_strains) {
        mvprintw(state->params.height + 2, 30, "Active strains:");
        int line = 3;
        for (int i = 0; i < MAX_STRAINS; i++) {
            if (state->strains[i].base_infectivity > 0.0f && state->strain_stats[i] > 0) {
                attron(COLOR_PAIR(state->strains[i].color_pair));
                mvprintw(state->params.height + line, 30, "%c: %d (inf:%.2f sev:%.2f mut:%.4f)",
                         state->strains[i].symbol, state->strain_stats[i], state->strains[i].base_infectivity,
                         state->strains[i].severity, state->strains[i].mutation_rate);
                attroff(COLOR_PAIR(state->strains[i].color_pair));
                line++;
            }
        }
    }
    attroff(COLOR_PAIR(10));
}

void display_legend(const SimulationState* state) {
    int start_x = 110;
    int start_y = state->params.height + 2;

    attron(COLOR_PAIR(11));
    mvprintw(start_y, start_x, "=== Legend ===");
    attroff(COLOR_PAIR(11));

    attron(COLOR_PAIR(10));
    mvprintw(start_y + 1, start_x, "Healthy: .     Vaccinated: V");

    if (state->params.enable_multiple_strains) {
        mvprintw(start_y + 2, start_x, "Infected (by strain):");

        int row_offset = 3;
        int strains_per_row = 4;

        for (int i = 0; i < MAX_STRAINS; i++) {
            if (state->strains[i].base_infectivity > 0.0f) {
                int row = start_y + row_offset + i / strains_per_row;
                int col_offset = (i % strains_per_row) * 20;

                attron(COLOR_PAIR(state->strains[i].color_pair));
                mvprintw(row, start_x + col_offset, "%c - strain %c", state->strains[i].symbol, 'A' + i);
                attroff(COLOR_PAIR(state->strains[i].color_pair));
            }
        }

        int active_strains = 0;
        for (int i = 0; i < MAX_STRAINS; i++) {
            if (state->strains[i].base_infectivity > 0.0f) active_strains++;
        }
        int last_strain_row = start_y + 3 + (active_strains - 1) / 4;
        mvprintw(last_strain_row + 1, start_x, "Immune stages: F,S,W,w");
    } else {
        mvprintw(start_y + 3, start_x, "Infected: I");
        mvprintw(start_y + 4, start_x, "Immune stages: F,S,W,w");
    }

    mvprintw(start_y + (state->params.enable_multiple_strains ? 5 : 6), start_x,
             "Controls: P-pause S-stats V-vaccine");
    mvprintw(start_y + (state->params.enable_multiple_strains ? 6 : 7), start_x,
             "         M-mutation R-restart Q-quit");
    mvprintw(start_y + (state->params.enable_multiple_strains ? 8 : 9), start_x,
             "Vaccines: 1-Standard 2-Broad 3-Booster 4-Adaptive");
    mvprintw(start_y + (state->params.enable_multiple_strains ? 9 : 10), start_x,
             "Mass vacc: T-now +/-interval [/]coverage");
    attroff(COLOR_PAIR(10));
}

void display_vaccination_stats(const SimulationState* state) {
    const VaccinationSystem* vs = &state->vaccination;

    if (!vs->enable_vaccination) return;

    int start_y = state->params.height + 2;
    int start_x = 75;

    attron(COLOR_PAIR(11));
    mvprintw(start_y, start_x, "=== VACCINATION SYSTEM ===");
    attroff(COLOR_PAIR(11));

    attron(COLOR_PAIR(10));

    const Vaccine* current = &vs->current_vaccine;
    attron(COLOR_PAIR(current->color_pair));
    mvprintw(start_y + 1, start_x, "Current vaccine: %s (%c)", current->name, current->symbol);
    attroff(COLOR_PAIR(current->color_pair));

    mvprintw(start_y + 2, start_x, "Effectiveness: %.1f%%", current->effectiveness * 100);
    mvprintw(start_y + 3, start_x, "Duration: %.0f-%.0f gen", current->duration_min, current->duration_max);
    mvprintw(start_y + 4, start_x, "Mutation resistance: %.1f%%", current->mutation_resistance * 100);

    mvprintw(start_y + 6, start_x, "Currently vaccinated: %d", vs->current_vaccinated);
    mvprintw(start_y + 7, start_x, "Total vaccinated: %d", vs->total_vaccinated);
    mvprintw(start_y + 8, start_x, "Mass vaccinations: %d", vs->mass_vaccination_count);

    int generations_since_last = state->generation - vs->last_mass_vaccination_gen;
    int generations_to_next = vs->mass_vaccination_interval - generations_since_last;

    if (generations_to_next <= 0) {
        mvprintw(start_y + 9, start_x, "Next mass vacc: READY!");
    } else {
        mvprintw(start_y + 9, start_x, "Next mass vacc: %d gen", generations_to_next);
    }

    attroff(COLOR_PAIR(10));
}