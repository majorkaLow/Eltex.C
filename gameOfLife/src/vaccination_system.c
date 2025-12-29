#include "../include/vaccination_system.h"

#include <ncurses.h>
#include <string.h>

#include "../include/cell_states.h"
#include "../include/immunity_system.h"
#include "../include/utils.h"

void init_vaccine(Vaccine* vaccine, VaccineType type, const char* name, float effectiveness,
                  float duration_min, float duration_max, float coverage, float mutation_resistance,
                  float cost, int color_pair, char symbol) {
    vaccine->type = type;
    strncpy(vaccine->name, name, sizeof(vaccine->name) - 1);
    vaccine->name[sizeof(vaccine->name) - 1] = '\0';
    vaccine->effectiveness = clamp_float(effectiveness, 0.0f, 1.0f);
    vaccine->duration_min = duration_min;
    vaccine->duration_max = duration_max;
    vaccine->coverage = clamp_float(coverage, 0.0f, 1.0f);
    vaccine->mutation_resistance = clamp_float(mutation_resistance, 0.0f, 1.0f);
    vaccine->cost = cost;
    vaccine->color_pair = color_pair;
    vaccine->symbol = symbol;
}

void init_vaccination_system(VaccinationSystem* vs) {
    memset(vs, 0, sizeof(VaccinationSystem));

    vs->enable_vaccination = 1;
    vs->enable_mass_vaccination = 1;
    vs->last_mass_vaccination_gen = -MASS_VACCINATION_INTERVAL;

    vs->spontaneous_rate = VACCINATION_SPONTANEOUS_RATE;
    vs->loss_rate = VACCINATION_LOSS_RATE;

    vs->mass_vaccination_interval = MASS_VACCINATION_INTERVAL;
    vs->mass_vaccination_coverage = MASS_VACCINATION_COVERAGE;

    init_vaccine(&vs->available_vaccines[VACCINE_STANDARD], VACCINE_STANDARD, "Standard", 0.9f, 50.0f, 100.0f,
                 0.8f, 0.3f, 1.0f, 9, 'S');

    init_vaccine(&vs->available_vaccines[VACCINE_BROAD_SPECTRUM], VACCINE_BROAD_SPECTRUM, "Broad Spectrum",
                 0.7f, 40.0f, 80.0f, 0.7f, 0.6f, 1.5f, 10, 'B');

    init_vaccine(&vs->available_vaccines[VACCINE_BOOSTER], VACCINE_BOOSTER, "Booster", 0.95f, 30.0f, 60.0f,
                 0.9f, 0.2f, 2.0f, 11, 'T');

    init_vaccine(&vs->available_vaccines[VACCINE_ADAPTIVE], VACCINE_ADAPTIVE, "Adaptive", 0.8f, 60.0f, 120.0f,
                 0.6f, 0.8f, 2.5f, 12, 'A');

    vs->current_vaccine = vs->available_vaccines[VACCINE_STANDARD];
}

float calculate_vaccine_effectiveness(const Vaccine* vaccine, const VirusStrain* strain) {
    float effectiveness = vaccine->effectiveness;

    switch (vaccine->type) {
        case VACCINE_STANDARD:
            if (strain->id == 0)
                effectiveness *= 1.2f;
            else
                effectiveness *= 0.6f;
            break;

        case VACCINE_BROAD_SPECTRUM:
            effectiveness *= 0.9f;
            break;

        case VACCINE_BOOSTER:
            effectiveness *= 1.3f;
            break;

        case VACCINE_ADAPTIVE:
            float mutation_factor = 1.0f + strain->mutation_rate * vaccine->mutation_resistance * 5.0f;
            effectiveness *= mutation_factor;
            break;
        default:
            break;
    }

    effectiveness *= (1.0f - strain->base_infectivity * 0.2f);
    effectiveness *= (1.0f + strain->severity * 0.3f);

    return clamp_float(effectiveness, 0.1f, 0.95f);
}

void apply_vaccine_to_cell(Cell* cell, const Vaccine* vaccine, SimulationState* state) {
    cell->state = STATE_VACCINATED;
    cell->vaccine_timer = random_range((int)vaccine->duration_min, (int)vaccine->duration_max);
    cell->vaccine_type = vaccine->type;

    for (int i = 0; i < MAX_STRAINS; i++) {
        if (state->strains[i].base_infectivity > 0.0f) {
            VirusStrain* strain = &state->strains[i];
            float immunity_chance = calculate_vaccine_effectiveness(vaccine, strain);

            if (random_float() < immunity_chance) {
                set_immunity_to_strain(cell, i);
            }
        }
    }
}

void perform_mass_vaccination(SimulationState* state) {
    VaccinationSystem* vs = &state->vaccination;

    if (!vs->enable_vaccination || !vs->enable_mass_vaccination) {
        return;
    }

    int generations_since_last = state->generation - vs->last_mass_vaccination_gen;

    if (generations_since_last >= vs->mass_vaccination_interval) {
        int vaccinated_this_round = 0;
        Vaccine* current_vaccine = &vs->current_vaccine;

        for (int y = 0; y < state->params.height; y++) {
            for (int x = 0; x < state->params.width; x++) {
                Cell* cell = &state->world[y][x];

                if ((cell->state == STATE_HEALTHY || cell->state == STATE_IMMUNE ||
                     cell->state == STATE_VACCINATED) &&
                    random_float() < vs->mass_vaccination_coverage * current_vaccine->coverage) {
                    apply_vaccine_to_cell(cell, current_vaccine, state);
                    vaccinated_this_round++;
                }
            }
        }

        if (vaccinated_this_round > 0) {
            vs->current_vaccinated += vaccinated_this_round;
            vs->total_vaccinated += vaccinated_this_round;
            vs->mass_vaccination_count++;
            vs->last_mass_vaccination_gen = state->generation;

            if (state->show_stats) {
                attron(COLOR_PAIR(current_vaccine->color_pair));
                mvprintw(state->params.height + 15, 0, "MASS VACCINATION: %s - %d cells vaccinated",
                         current_vaccine->name, vaccinated_this_round);
                attroff(COLOR_PAIR(current_vaccine->color_pair));
            }
        }
    }
}

void process_vaccinated_cell(SimulationState* state, int y, int x) {
    Cell* current = &state->world[y][x];
    Cell* next = &state->next_world[y][x];

    if (next->vaccine_timer > 0) {
        next->vaccine_timer--;

        if (next->vaccine_timer == 0) {
            next->state = STATE_HEALTHY;
            state->vaccination.current_vaccinated--;

            for (int i = 0; i < MAX_STRAINS; i++) {
                if (has_immunity_to_strain(current, i) && random_float() < state->vaccination.loss_rate) {
                    int byte_idx = i / 8;
                    int bit_idx = i % 8;
                    next->immunity_to_strain[byte_idx] &= ~(1 << bit_idx);
                }
            }
        }
    }

    next->age = current->age + 1;

    if (current->age > 50 && random_float() < 0.0005f) {
        next->state = STATE_DEAD;
        state->vaccination.current_vaccinated--;
        return;
    }

    try_reproduce_cell(state, y, x);
}