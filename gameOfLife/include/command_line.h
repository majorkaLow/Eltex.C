#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include "../structures/types.h"

void parse_command_line_arguments(int argc, char* argv[], SimulationState* state);
void print_help_message(void);

#endif