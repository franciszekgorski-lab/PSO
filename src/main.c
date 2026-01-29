#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "map.h"
#include "logger.h"
#include "pso.h"

#define LOG_PATH "logs.csv"

int main(int argc, char** argv) {

        int do_save = 1;
        int do_generate = 0;
        int particle_number = 100;

        FILE * log_fptr = fopen(LOG_PATH, "w");
        if (log_fptr == NULL) {
                printf(" EROROROROROROR \n");
                return 1 ;
        }


        pso_settings pso_s = (pso_settings){
                1,1,1
        };
        log_settings log_s = (log_settings){
                100, 0
        };

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        //nwm czy to chcemy 
        if(argc < 10){
                printf("Zbyt mało argumentów");
                return 1;
        }

        //ładowanie mapy mozna zostawić tak ale można też dodać generacje gdy nie uda sie jej zaladowac
        Map* map = NULL;
        map = Map_Load(argv[1]);
        if ( map == NULL ) {
                printf("Brak mapy!\n");
                return 1;
        }
        

        int i = 2; 
        while((i<argc) && (i+1 < argc)){
                if(argv[i][0] != '-'){
                        printf(" Zly format argumentow wywolania ");
                        return 1;
                }
                switch(argv[i][1]){
                        case 'p':
                                particle_number = atoi(argv[i+1]);
                                printf("particle number %d \n", particle_number);
                                break;
                        case 'c':
                                pso_s = read_pso_settings(argv[i+1]);
                                printf("pso set w %lf c1 %lf c2 %lf \n", pso_s.w, pso_s.c1, pso_s.c2);
                                break;
                        case 'n':
                                log_s.log_ite = atoi(argv[i+1]);
                                printf("log frequency %d \n", log_s.log_ite);
                                break;
                        case 'i':
                                log_s.ite = atoi(argv[i+1]);
                                printf("all iterations %d\n", log_s.ite);
                                break;
                }
                i+=2;
        }

        swarm * s = swarm_construct(particle_number, map);

        // generowanie mapy
        if ( do_generate ) Map_Generate(map);

        //rysowanie mapy

        //Map_Visualize(map);
        // Pętla roju 
        i = 0;
        log_headers(log_fptr,s);
        while(i<log_s.ite){
                if(i % log_s.log_ite == 0){
                        log_positions(log_fptr, s, i / log_s.log_ite);
                }
                i++;
                update_particles(s,map,pso_s);
        }
        log_results(log_fptr,s);
        fclose(log_fptr);
        //zapisywanie mapy
        if ( do_save ) Map_Save(map);
        Map_Destroy(map);
        destroy_swarm(s);
        return 0;
}
