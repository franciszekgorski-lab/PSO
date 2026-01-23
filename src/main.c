#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#include "map.h"

int main(int argc, char** argv) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        Map* map = Map_LoadFromSettings(argv[1]);
        
        if ( map == NULL ) {
                printf("Brak mapy!\n");
                return 1;
        }

        if ( strcmp(argv[2], "generate") == 0 ) Map_Generate(map);
        else if ( strcmp(argv[2], "load") == 0 ) {
                if ( argc != 4 ) {
                        printf("Nie podano nazwy pliku z zapisem mapy!\n");
                        return 1;
                }
                
                if ( Map_Load(map, argv[3]) ) return 1;
        } else    printf("Wrong second argument; [generate/load]\n");

        Map_Visualize(map);
        Map_Save(map);
        
        Map_Destroy(map);
        SDL_Quit();
        return 0;
}
