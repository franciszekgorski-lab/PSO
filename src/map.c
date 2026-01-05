#include "map.h"
#include "utils.h"

#include <stdlib.h>
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

        int peak_count = (rand() % 5) + 2;

}
