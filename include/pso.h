#ifndef PSO
#define PSO

typedef struct _map Map;

typedef struct {
        double w, c1, c2;
} pso_settings;

typedef struct{
    float pos_x;  //składowa x pozycji
    float pos_y;    // skladowa y
    float vel_x;  // składowa x prędkości danej czastki
    float vel_y;  // to samo tylko y
    int highest_found_x; // indeks najniższej głebi znalezionej przez tą cząstkę.
    int highest_found_y;
} particle;

typedef struct swarm{
    particle ** particle_arr;
    int size;
    int highest_found_x;
    int highest_found_y;
} swarm;

pso_settings read_pso_settings(const char* file_path);

swarm* swarm_construct(int particle_number, Map * map);

void update_particles(swarm * s, Map *m, pso_settings pso_s);

void destroy_swarm(swarm * s);

#endif
