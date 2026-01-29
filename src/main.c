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
        //flagi do zapisywania i generowania mapy
        int do_save = 0;
        //zmienna odpowiadajaca za domyslna ilosc czastek
        int particle_number = 100;

        FILE * log_fptr = fopen(LOG_PATH, "w");
        if (log_fptr == NULL) {
                printf(" EROROROROROROR \n");
                return 1 ;
        }

        // domyslne wartosci ustawien algorytmu pso - w, c1, c2
        pso_settings pso_s = (pso_settings){
                1,1,1
        };
        // domyslne wartosci utawien loggera - ilosc wszystkich iteracji i czestotoliwosc loggowania
        log_settings log_s = (log_settings){
                100, 0
        };
        
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        //to chyba trzeba bedzie usunac
        if(argc < 10){
                printf("Zbyt mało argumentów");
                return 1;
        }

        Map* map = NULL;

        int opt;
        while((opt = getopt(argc, argv, "l:p:c:n:i:sg:")) != -1){
                switch(opt){
                        case 'l':
                                map = Map_Load(optarg);
                                break;
                        case 'p':
                                particle_number = atoi(optarg);
                                printf("particle number %d \n", particle_number);
                                break;
                        case 'c':
                                pso_s = read_pso_settings(optarg);
                                printf("pso set w %lf c1 %lf c2 %lf \n", pso_s.w, pso_s.c1, pso_s.c2);
                                break;
                        case 'n':
                                log_s.log_ite = atoi(optarg);
                                printf("log frequency %d \n", log_s.log_ite);
                                break;
                        case 'i':
                                log_s.ite = atoi(optarg);
                                printf("all iterations %d\n", log_s.ite);
                                break;
                        case 's':
                                do_save = 1;
                                break;
                        case 'g':
                                map = Map_LoadFromSettings(optarg);
                                Map_Generate(map);
                                break;
                        default:
                                break;
                }
        
        }
        
        //ładowanie mapy mozna zostawić tak ale można też dodać generacje gdy nie uda sie jej zaladowac
        if ( map == NULL ) {
                printf("Brak mapy!\n");
                return 1;
        }

        //incijalizujemy swarm - S z PSO
        swarm * s = swarm_construct(particle_number, map);

        //rysowanie mapy
        Map_Visualize(map, s);
       
        // Pętla roju 
        int i = 0;
        log_headers(log_fptr,s);
        while(i<log_s.ite){
                if(i % log_s.log_ite == 0){
                        log_positions(log_fptr, s, i / log_s.log_ite);
                }
                i++;
                //odpowiada za predkosci kazdej czastki i aktualiowanie pBest i gBest
                Map_Visualize(map, s);
                update_particles(s,map,pso_s);
                SDL_Delay(200); 
        }
        log_results(log_fptr,s);
        fclose(log_fptr);
       
        //zapisywanie mapy
        if ( do_save ) Map_Save(map);

        int running = 1;
        SDL_Event event;
        while (running) {
                while(SDL_PollEvent(&event)) {
                        switch(event.type) {
                        case SDL_QUIT:
                                running = 0;
                                break;
                        default: 
                                break;
                        }
                }
        }
               
        Map_Destroy(map);
        destroy_swarm(s);
       
        return 0;
}
