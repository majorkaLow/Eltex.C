#ifndef DISPLAY_SYSTEM_H
#define DISPLAY_SYSTEM_H

#include <ncurses.h>

#include "../structures/types.h"

void init_display_colors(void);
bool check_terminal_size(int width, int height);

int get_cell_color(const Cell* cell);
char get_cell_character(const Cell* cell);

void display_world(const SimulationState* state);
void display_statistics(const SimulationState* state);
void display_legend(const SimulationState* state);
void display_vaccination_stats(const SimulationState* state);

#endif