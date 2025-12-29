#include "../include/immunity_system.h"

#include <string.h>

bool has_immunity_to_strain(const Cell* cell, int strain_id) {
    if (strain_id < 0 || strain_id >= MAX_STRAINS) return false;

    int byte_idx = strain_id / 8;
    int bit_idx = strain_id % 8;
    return (cell->immunity_to_strain[byte_idx] >> bit_idx) & 1;
}

void set_immunity_to_strain(Cell* cell, int strain_id) {
    if (strain_id < 0 || strain_id >= MAX_STRAINS) return;

    int byte_idx = strain_id / 8;
    int bit_idx = strain_id % 8;
    cell->immunity_to_strain[byte_idx] |= (1 << bit_idx);
}

void clear_immunity(Cell* cell) { memset(cell->immunity_to_strain, 0, sizeof(cell->immunity_to_strain)); }