#ifndef PSO
#define PSO

#include "map.h"

typedef struct{
    float pos_x;  //składowa x pozycji
    float pos_y;    // skladowa y
    float vel_x;  // składowa x prędkości danej czastki
    float vel_y;  // to samo tylko y
    int lowest_found_x; // indeks najniższej głebi znalezionej przez tą cząstkę.
    int lowest_found_y;
} particle;

typedef struct{
    particle ** particle_arr;
    int size;
    int lowest_found_x;
    int lowest_found_y;
} swarm;

swarm* swarm_construct(int particle_number, Map * map);

void update_particles(swarm * s, Map *m);

void destroy_swarm(swarm * s)

#endif
