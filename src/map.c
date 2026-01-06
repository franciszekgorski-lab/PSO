#include "map.h"
#include "utils.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

Map* Map_Construct(int w, int h, int max_d) {
        Map* temp = malloc(sizeof(Map));
        temp->width = w;
        temp->heigth = h;
        temp->max_depth = max_d;
        temp->depth = Vector_Construct(w * h);

        for (int i = 0; i < temp->depth->size; i++)      temp->depth->value[i] = 0;
        return temp;
}

void Map_Generate(Map* map, int multi) {
        srand(time(NULL));

        int peak_count = 5 + multi + (rand() % (10 + multi));

        for (int i = 0; i < peak_count; i++) {
                Vector* dists = Vector_Construct(0);
                Vector* indexs = Vector_Construct(0);

                int index = rand() % map->depth->size;
                int radius = 30 + (rand() % 100);
                double max = (rand() % (map->max_depth * 2)) - (map->max_depth);

                if (max == 0) {
                        i--;
                        continue;
                }

                for (int j = 0; j < map->depth->size; j++) {
                        if ( Map_GetDist(map, index, j) <= (double)radius) {
                                Vector_Append(indexs, j);
                                Vector_Append(dists, Map_GetDist(map, index, j));
                        }
                }

                for (int j = 0; j < indexs->size; j++) {
                        double depth = max / radius;
                        map->depth->value[(int)indexs->value[j]] -= max - depth * dists->value[j];
                }
        }

}

double Map_GetDist(Map* map, int index0, int index1) {
        int x0 = index0 % map->width;
        int x1 = index1 % map->width;        
        double y0 = (index0 - x0)/ map->width;
        double y1 = (index1 - x1) / map->width;

        double x = fabs(x0 - x1);
        double y = fabs(y0 - y1);

        return sqrt(x*x + y*y);
}

void Map_Print(Map* map) {
        for (int i = 0; i < map->depth->size; i++) {
                printf("%3.1f ", map->depth->value[i]);

                if ( (i + 1) % map->width == 0 ) printf("\n");
        }
}

void Map_Visualize(Map* map) {
        SDL_Window* window = SDL_CreateWindow("PSO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, map->width + 180, map->heigth + 100, SDL_WINDOW_RESIZABLE);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Event  event;
        int running = 1;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < map->depth->size; i++) {
                double amp_p = 1500.0;
                double amp_d = 1500.0;
                double depth = map->depth->value[i];
                
                int r;
                int g;
                int b;

                if (depth > 0) {
                        r = 170 + (depth / amp_p) * 80;
                        g = 200 - (depth / amp_p) * 30;
                        b = 0;
                } else if (depth < 0) {
                        depth *= -1;
                        r = 170 - (depth / amp_d) * 80;
                        g = 200 - (depth / amp_p) * 30;
                        b = (depth / amp_d) * 40;
                } else {
                        r = 170;
                        b = 0;
                        g = 200;
                }

                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, (i % map->width) + 80, i / map->width);
        }

        SDL_RenderPresent(renderer);

        while ( running == 1 ) {
                while( SDL_PollEvent(&event) ) {
                        switch(event.type) {
                        case SDL_QUIT:
                                running = 0;
                                break;
                        default:
                                break;
                        }
                }
        }
}
