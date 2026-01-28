#include "pso.h"
#include <time.h>
#include <stdlib.h>

swarm* swarm_construct(int particle_number, Map * map){
    srand(time);
    swarm * s = malloc(sizeof(swarm*));
    s->size = particle_number;
    s->lowest_found = 0; /// TU TRZEBA ZMNIENIC NA NAJWIEKSZA MOZLIWA PRZY GENERACJI WARTOSC
    s->particle_arr = malloc(sizeof(particle*)*particle_number);

    for(int i =0; i<particle_number; i++){
        s->particle_arr[i] = malloc(sizeof(particle));
        s->particle_arr[i]->pos_x = rand()%(map->width);
        s->particle_arr[i]->pos_x = rand()%(map->height);
        s->particle_arr[i]->vel_x = 0;
        s->particle_arr[i]->vel_y = 0;
        s->particle_arr[i]->lowest_found = 0; // TU TRZEBA ZMNIENIC NA NAJWIEKSZA MOZLIWA PRZY GENERACJI WARTOSC
    }
    return s;
}
// return 1 - jeszcze jakies czastki pozostaja w ruchu
// TU JAKO ARGUMENTY TRZEBA DODAC WAGI I WPISAC JE W ROWNANIA
int update_particles(swarm * s, Map *m){
    //dla każdej cząstki
    for(int i =0; i<particle_number; i++){
        particle * p =  s->particle_arr[i];
        //zmieniamy pozycje o predkosc
        p->pos_x += p->vel_x;
        p->pos_y += p->vel_y;
        int ind = (int)p->pos_x + (int)p->pos_y * m->width;
        int lowest_ind = p->lowest_found_x + p->lowest_found_y * m->width;
        //sprawdzamy czy nowa pozycja to jest jakieś minimum
        // tutaj lokalne dla cząstki
        if(m->depth->value[ind] < m->depth->value[lowest_ind]){
            p->lowest_found_x = (int)p->pos_x;
            p->lowest_found_y = (int)p->pos_y * m->width;
        }
        int lowest_ind = s->lowest_found_x + s->lowest_found_y * m->width;
        //tutaj globalne
        if(m->depth->value[ind] < m->depth->value[lowest_ind]){
            s->lowest_found_x = (int)p->pos_x;
            s->lowest_found_y = (int)p->pos_y * m->width;
        }
        //losujemy prędkość - składowa x
        float r1 = (float)rand()/(float)(RAND_MAX);
        float r2 = (float)rand()/(float)(RAND_MAX);
        p->vel_x = 1 * vel_x + r1 * 1 * (p->lowest_found_x - p->pos_x) + r2 * 1 * (s->lowest_found_x - p->pos_x);
        //jesli spowoduje ona, ze czastka wychodzi za mape to zmieniamy kierunek na przeciwny
        if(p->pos_x + p->vel_x > m->width){
            p->vel_x *= -1;
        }
        r1 = (float)rand()/(float)(RAND_MAX);
        r2 = (float)rand()/(float)(RAND_MAX);
        p->vel_y = 1*vel_y + r1 * 1 * (p->lowest_found_y - p->pos_y) + r2 * 1 * (s->lowest_found_y - p->pos_y);
        //to samo dla skladowej y
        if(p->pos_y + p->vel_y > m->height){
            p->vel_y *= -1;
        }
    }
}

void destroy_swarm(swarm * s){
    for(int i = 0; i< s->size; i++){
        free(particle_arr[i]);
    }
    free(s->particle_arr);
    free(s);
}