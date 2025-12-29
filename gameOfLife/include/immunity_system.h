#ifndef IMMUNITY_SYSTEM_H
#define IMMUNITY_SYSTEM_H

#include <stdbool.h>

#include "../structures/types.h"

bool has_immunity_to_strain(const Cell* cell, int strain_id);
void set_immunity_to_strain(Cell* cell, int strain_id);
void clear_immunity(Cell* cell);

#endif