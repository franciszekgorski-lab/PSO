#ifndef GENEROWANIE_MAPY
#define GENEROWANIE_MAPY

#include "utils.h"

typedef struct _map{
        int width;
        int heigth;
        int max_depth;
        Vector* depth;
} Map;

Map* Map_Construct(int w, int h, int max_d);
void Map_Generate(Map* map, int multi);
double Map_GetDist(Map* map, int index0, int index1);
void Map_Print(Map* map);
void Map_Visualize(Map* map);
void Map_Destroy(Map* map);

#endif
