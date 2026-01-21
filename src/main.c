#include <SDL2/SDL.h>
#include <stdio.h>

#include "map.h"

int main(int argc, char** argv) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        if (argc != 2) return 1;

        Map* map = Map_LoadFromSettings(argv[1]);

        Map_Generate(map);

        Map_Visualize(map);
        
        Map_Destroy(map);

        SDL_Quit();
        return 0;
}
