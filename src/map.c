#include "map.h"
#include "utils.h"

Map* Map_Construct(int w, int h, int max_d, int max_p) {
        Map* temp = malloc(sizeof(Map));
        temp->width = w;
        temp->heigth = h;
        temp->max_depth = max_d;
        temp->max_peak = max_p;
        temp->size = w * h;
        temp->depth = Vector_Construct(temp->size);
}
