#ifndef LOGGER
#define LOGGER

#include "pso.h"
#include <stdio.h>

typedef struct {
        int ite, log_ite; // ite - wszystkie iteracje, log_ite - co ile iteracji przepisywać dane do pliku logowego
} log_settings;

void log_headers(FILE * fptr, swarm * s);

void log_positions(FILE * fptr, swarm * s, int iteration);

void log_results(FILE * fptr, swarm * s);
#endif
