#ifndef VACCINATION_SYSTEM_TYPES_H
#define VACCINATION_SYSTEM_TYPES_H

#include "vaccine_types.h"

typedef struct {
    int enable_vaccination;
    int enable_mass_vaccination;

    int last_mass_vaccination_gen;

    float spontaneous_rate;
    float loss_rate;

    int mass_vaccination_interval;
    float mass_vaccination_coverage;

    Vaccine current_vaccine;
    Vaccine available_vaccines[VACCINE_TYPE_COUNT];

    int total_vaccinated;
    int current_vaccinated;
    int mass_vaccination_count;
} VaccinationSystem;

#endif