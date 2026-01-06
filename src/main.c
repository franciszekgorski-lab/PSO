#include <SDL2/SDL.h>
#include <stdio.h>

#include "map.h"

int main(int argc, char** argv) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        if (argc != 6) return 1;

        int w           = atoi(argv[1]);
        int h           = atoi(argv[2]);
        int max_depth   = atoi(argv[3]);
        int max_peak    = atoi(argv[4]);
        int multi       = atoi(argv[5]);

        Map* map = Map_Construct(w, h, max_depth, max_peak);

        Map_Generate(map, multi);

        Map_Visualize(map);

        SDL_Quit();
        return 0;
}
