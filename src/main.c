#include <SDL2/SDL.h>
#include <stdio.h>

#include "map.h"

int main(int argc, char** argv) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        if (argc != 5) return 1;

        int w           = atoi(argv[1]);
        int h           = atoi(argv[2]);
        int max_depth   = atoi(argv[3]);
        int multi       = atoi(argv[4]);

        Map* map = Map_Construct(w, h, max_depth);

        Map_Generate(map, multi);

        Map_Visualize(map);
        
        Map_Destroy(map);

        SDL_Quit();
        return 0;
}
