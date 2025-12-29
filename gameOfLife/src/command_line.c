#include "../include/command_line.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

void parse_command_line_arguments(int argc, char* argv[], SimulationState* state) {
    static struct option long_options[] = {{"width", required_argument, 0, 'w'},
                                           {"height", required_argument, 0, 'h'},
                                           {"delay", required_argument, 0, 'd'},
                                           {"infection-rate", required_argument, 0, 'i'},
                                           {"healthy-rate", required_argument, 0, 'H'},
                                           {"vaccination-rate", required_argument, 0, 'V'},
                                           {"vaccination-eff", required_argument, 0, 'e'},
                                           {"strains", required_argument, 0, 's'},
                                           {"log-file", required_argument, 0, 'l'},
                                           {"no-mutations", no_argument, 0, 'M'},
                                           {"no-vaccination", no_argument, 0, 'N'},
                                           {"no-multiple-strains", no_argument, 0, 'S'},
                                           {"help", no_argument, 0, '?'},
                                           {0, 0, 0, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "w:h:d:i:H:V:e:s:l:MNS?", long_options, NULL)) != -1) {
        switch (opt) {
            case 'w':
                state->params.width = clamp(atoi(optarg), 10, 200);
                break;

            case 'h':
                state->params.height = clamp(atoi(optarg), 10, 100);
                break;

            case 'd':
                state->params.delay_ms = clamp(atoi(optarg), 10, 1000);
                break;

            case 'i':
                state->params.initial_infection_rate = clamp_float(atof(optarg), 0.0f, 1.0f);
                break;

            case 'H':
                state->params.initial_healthy_rate = clamp_float(atof(optarg), 0.0f, 1.0f);
                break;

            case 'V':
                state->params.vaccination_rate = clamp_float(atof(optarg), 0.0f, 1.0f);
                break;

            case 'e':
                state->params.vaccination_effectiveness = clamp_float(atof(optarg), 0.0f, 1.0f);
                break;

            case 's':
                state->params.max_strains = clamp(atoi(optarg), 1, MAX_STRAINS);
                break;

            case 'l':
                state->params.log_to_file = 1;
                strncpy(state->params.stats_filename, optarg, sizeof(state->params.stats_filename) - 1);
                break;

            case 'M':
                state->params.enable_mutations = 0;
                break;

            case 'N':
                state->params.enable_vaccination = 0;
                state->vaccination.enable_vaccination = 0;
                break;

            case 'S':
                state->params.enable_multiple_strains = 0;
                break;

            case '?':
                state->show_help = 1;
                break;
        }
    }
}

void print_help_message(void) {
    printf("Epidemic Life - Extended Simulation\n\n");
    printf("Usage: epidemic_life [OPTIONS]\n\n");
    printf("Options:\n");
    printf("  -w, --width=N           World width (10-200, default: 80)\n");
    printf("  -h, --height=N          World height (10-100, default: 40)\n");
    printf("  -d, --delay=N           Delay between generations in ms (10-1000)\n");
    printf("  -i, --infection-rate=F  Initial infection rate (0-1)\n");
    printf("  -H, --healthy-rate=F    Initial healthy rate (0-1)\n");
    printf("  -V, --vaccination-rate=F Vaccination rate (0-1)\n");
    printf("  -e, --vaccination-eff=F Vaccine effectiveness (0-1)\n");
    printf("  -s, --strains=N         Max number of strains (1-8)\n");
    printf("  -l, --log-file=FILE     Log statistics to CSV file\n");
    printf("  -M, --no-mutations      Disable virus mutations\n");
    printf("  -N, --no-vaccination    Disable vaccination\n");
    printf("  -S, --no-multiple-strains Disable multiple strains\n");
    printf("  -?, --help              Show this help message\n\n");
    printf("In-program controls:\n");
    printf("  P, Space   - Pause/Resume\n");
    printf("  S          - Toggle statistics\n");
    printf("  V          - Toggle vaccination\n");
    printf("  M          - Toggle mutations\n");
    printf("  R          - Restart simulation\n");
    printf("  Q          - Quit\n");
}