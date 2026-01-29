#include "pso.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//czyta z pliku 3 liczby ktore sa odpowiednimi argumentami dla pso - w,c1,c2
pso_settings read_pso_settings(const char* file_path){
    pso_settings ret =(pso_settings){1,1,1};
    FILE* fptr = fopen(file_path, "r");
    if (fptr == NULL) {
            printf("Błąd przy czytaniu mapy z pliku!\n");
            return (pso_settings){1,1,1}; //zwraca domyslna wartosc
    }

    fscanf(fptr,"%lf %lf %lf ", &ret.w, &ret.c1, &ret.c2);
    fclose(fptr);
    return ret;
}

//incjalizuje roj czastek
swarm* swarm_construct(int particle_number, Map * map){
    //seeduje random
    srand(time(NULL));
    swarm * s = malloc(sizeof(swarm));
    //inicjalizuje pola structa
    s->size = particle_number;
    s->highest_found_x = 0;
    s->highest_found_y = 0;
    s->particle_arr = malloc(sizeof(particle*)*particle_number);
    //inicjalizuje pola structa kazdej czastki
    for(int i =0; i<particle_number; i++){
        particle * p = malloc(sizeof(particle));
        
        p->pos_x = rand()%(map->width);
        p->pos_y = rand()%(map->heigth);
        p->vel_x = 0;
        p->vel_y = 0;
        p->highest_found_x = (int)(p->pos_x);
        p->highest_found_y = (int)(p->pos_y);

        //szuka najwyzszego pola ze wszystkich w ktorych wylosowały się cząstki
        int ind = (int)p->pos_x + (int)p->pos_y * map->width;
        int highest_ind = s->highest_found_x + s->highest_found_y * map->width;
        if(map->depth->value[ind] > map->depth->value[highest_ind]){
            s->highest_found_x = (int)p->pos_x;
            s->highest_found_y = (int)p->pos_y;
        }

        s->particle_arr[i] = p;
    }
    return s;
}
// funkcja odpowiada za aktualizowanie pozycji i predkosci kazdej czastki
void update_particles(swarm * s, Map *m, pso_settings pso_s){
    //dla każdej cząstki
    for(int i =0; i<s->size; i++){
        particle * p =  s->particle_arr[i];
        //losujemy prędkość - składowa x
        float r1 = (float)rand()/(float)(RAND_MAX);
        float r2 = (float)rand()/(float)(RAND_MAX);
        p->vel_x = pso_s.w * p->vel_x + r1 * pso_s.c1 * (p->highest_found_x - p->pos_x) + r2 * pso_s.c2 * (s->highest_found_x - p->pos_x);
        //jesli spowoduje ona, ze czastka wychodzi za mape to zmieniamy kierunek na przeciwny
        if((p->pos_x + p->vel_x > m->width) || (p->pos_x + p->vel_x < 0)){
            p->vel_x *= -1;
        }
        //losujemy predkosc - skladowa y
        r1 = (float)rand()/(float)(RAND_MAX);
        r2 = (float)rand()/(float)(RAND_MAX);
        p->vel_y = pso_s.w * p->vel_y + r1 * pso_s.c1 * (p->highest_found_y - p->pos_y) + r2 * pso_s.c2 * (s->highest_found_y - p->pos_y);
        //jesli spowoduje ona, ze czastka wychodzi za mape to zmieniamy kierunek na przeciwny
        if((p->pos_y + p->vel_y > m->heigth) || (p->pos_y + p->vel_y < 0)){
            p->vel_y *= -1;
        }

        //zmieniamy pozycje o predkosc
        p->pos_x += p->vel_x;
        p->pos_y += p->vel_y;

        int ind = (int)p->pos_x + (int)p->pos_y * m->width;
        int highest_ind = p->highest_found_x + p->highest_found_y * m->width;
        //sprawdzamy czy nowa pozycja to jest jakieś minimum
        // tutaj lokalne dla cząstki
        if(m->depth->value[ind] > m->depth->value[highest_ind]){
            p->highest_found_x = (int)p->pos_x;
            p->highest_found_y = (int)p->pos_y;
        }
        highest_ind = s->highest_found_x + s->highest_found_y * m->width;
        //tutaj globalne
        if(m->depth->value[ind] > m->depth->value[highest_ind]){
            s->highest_found_x = (int)p->pos_x;
            s->highest_found_y = (int)p->pos_y;
        }
    }
}
//freeuje wszystko
void destroy_swarm(swarm * s){
    for(int i = 0; i< s->size; i++){
        free(s->particle_arr[i]);
    }
    free(s->particle_arr);
    free(s);

}
