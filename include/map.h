#ifndef GENEROWANIE_MAPY
#define GENEROWANIE_MAPY

#include "utils.h"

typedef struct _map{
        int width;
        int heigth;
        int max_h;
        int min_h;
        int max_r;
        int min_r;
        int multi;
        Vector* depth;
} Map;

Map* Map_Construct(int w, int h, int x_r, int n_r, int x_h, int n_h, int m);
Map* Map_LoadFromSettings(const char* file_path);
void Map_Generate(Map* map);
int Map_Load(Map* map, const char* file_path);
double Map_GetDist(Map* map, int index0, int index1);
void Map_Print(Map* map);
void Map_Visualize(Map* map);
void Map_Destroy(Map* map);
void Map_Save(Map* map);

#endif
