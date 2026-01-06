#include "map.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

Map* Map_Construct(int w, int h, int max_d, int max_p) {
        Map* temp = malloc(sizeof(Map));
        temp->width = w;
        temp->heigth = h;
        temp->max_depth = max_d;
        temp->max_peak = max_p;
        temp->depth = Vector_Construct(w * h);

        return temp;
}

void Map_Generate(Map* map) {
        for (int i = 0; i < map->depth->size; i++)      map->depth->value[i] = 0;
        
        srand(time(NULL));

        int peak_count = 5 + (rand() % 10);

        for (int i = 0; i < peak_count; i++) {
                Vector* dists = Vector_Construct(0);
                Vector* indexs = Vector_Construct(0);

                int index = rand() % map->depth->size;
                int radius = 30 + (rand() % 100);
                double max = (rand() % ( (map->max_depth * -1) + map->max_peak )) + (map->max_depth);

                for (int j = 0; j < map->depth->size; j++) {
                        if ( Map_GetDist(map, index, j) <= (double)radius) {
                                Vector_Append(indexs, j);
                                Vector_Append(dists, Map_GetDist(map, index, j));
                        }
                }

                for (int j = 0; j < indexs->size; j++) {
                        int depth = max / radius;

                        if (max > 0) {
                                map->depth->value[(int)indexs->value[j]] += max - depth * dists->value[j];
                        } else {
                                map->depth->value[(int)indexs->value[j]] -= max - depth * dists->value[j];
                        }
                }
        }

}

double Map_GetDist(Map* map, int index0, int index1) {
        int x0 = index0 % map->width;
        int x1 = index1 % map->width;        
        int y0 = (index0 - x0)/ map->width;
        int y1 = (index1 - x1) / map->width;

        double x = fabs(x0 + x1);
        double y = fabs(y0 + y1);

        return sqrt(x*x + y*y);
}

void Map_Print(Map* map) {
        for (int i = 0; i < map->depth->size; i++) {
                printf("%3.1f ", map->depth->value[i]);
                if ( i % map->width == 0 ) printf("\n");
        }
}
