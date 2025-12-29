#ifndef WORLD_UPDATE_H
#define WORLD_UPDATE_H

#include "../structures/types.h"

void update_world(SimulationState* state);
void copy_world_state(SimulationState* state);
void apply_next_world(SimulationState* state);

#endif