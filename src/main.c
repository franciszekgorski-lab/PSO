#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "map.h"

static struct option long_options[] = {
        {"save",        required_argument,      0, 's'},
        {"help",        no_argument,            0, 'h'},
        {"load",        required_argument,      0, 'l'},
        {"settings",    required_argument,      0, 'o'},
        {0, 0, 0, 0}
};

int main(int argc, char** argv) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }
        Map* map = NULL;
        
        int opt;
        int option_index;
        int do_save = 0;
        int do_generate = 0;

        while ( (opt = getopt_long(argc, argv, "ho:l:s:", long_options, &option_index)) != -1 ) {
                switch (opt) {
                case 'h':
                        //wstawić instrukcję obsługi
                        return 1;
                        break;

                case 'o':
                        map = Map_LoadFromSettings(optarg);
                        do_generate = 1;
                        break;

                case 'l':
                        map = Map_Load(optarg);
                        break;

                case 's':
                        do_save = 1;
                        break;

                case '?':
                        printf("Nieznany argument!\n");
                        break;
                }
        }       

        if ( map == NULL ) {
                printf("Brak mapy!\n");
                return 1;
        }

        if ( do_generate ) Map_Generate(map);

        Map_Visualize(map);
        if ( do_save ) Map_Save(map);
        
        Map_Destroy(map);
        SDL_Quit();
        return 0;
}
