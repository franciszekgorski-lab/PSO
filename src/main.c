#include <SDL2/SDL.h>
#include <stdio.h>

#include "map.h"

int main(int argc, char** argv) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL_Init Error: %s\n", SDL_GetError());
                return 1;
        }

        Map* map = Map_Construct(100, 100, 50, 50);

        Map_Generate(map);

        Map_Print(map);

        SDL_Quit();
        return 0;
}
