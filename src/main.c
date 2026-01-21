#include <SDL2/SDL.h>
#include <stdio.h>

#include "map.h"

int main(int argc, char** argv) {

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        if (argc != 6) return 1;
        if (argv[3] == 0) return 1;

        int w           = atoi(argv[1]);
        int h           = atoi(argv[2]);
        int max_r       = atoi(argv[3]);
        int max_h       = atoi(argv[4]);
        int multi       = atoi(argv[5]);

        Map* map = Map_Construct(w, h, max_h, max_r);

        Map_Generate(map, multi);

        Map_Visualize(map);
        
        Map_Print(map);

        Map_Destroy(map);

        SDL_Quit();
        return 0;
}
