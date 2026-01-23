#include "map.h"
#include "utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 800

// Inicjalizacja struktury Mapy,
//      zawiera ona wszystko potrzebne do stworzenia mapy "trzy wymiarowej" z rzutem z góry.
//      Implementacją tej "mapy" jest tablica zmiennych double, w każdej komórce tablicy
//      znajdziesz wartość depth (ang. głebokość) co odnosi do się ogólnego położenia
//      punktu względem poziomu zerowego, nazwa depth nie jest więc zbyt domyślna,
//      ale taka już jest.
Map* Map_Construct(int w, int h, int x_r, int n_r, int x_h, int n_h, int m) {
        Map* temp = malloc(sizeof(Map));
        temp->width = w;
        temp->heigth = h;
        temp->max_h = x_h;
        temp->min_h = n_h;
        temp->max_r = x_r;
        temp->min_r = n_r;
        temp->multi = m;
        temp->depth = Vector_Construct(w * h);

        for (int i = 0; i < temp->depth->size; i++)      temp->depth->value[i] = 0;
        return temp;
}

Map* Map_LoadFromSettings(const char* file_path) {
        FILE* settings = fopen(file_path, "r");
        char buffer[40];
        int value;

        int width;
        int heigth;
        int h_max;
        int h_min;
        int radius_max;
        int radius_min;
        int multi;

        if (settings == NULL) {
                printf("Błąd wczytywania ustawień mapy z pliku!\n");        
                return NULL;
        }

        while ( fscanf(settings, "%s %d", buffer, &value) == 2) {
                if (strcmp(buffer, "width") == 0) width = value;
                if (strcmp(buffer, "heigth") == 0) heigth = value;
                if (strcmp(buffer, "h_max") == 0) h_max = value;
                if (strcmp(buffer, "h_min") == 0) h_min = value;
                if (strcmp(buffer, "radius_max") == 0) radius_max = value;
                if (strcmp(buffer, "radius_min") == 0) radius_min = value;
                if (strcmp(buffer, "multi") == 0) multi = value;
        }

        Map* temp = Map_Construct(width, heigth, radius_max, radius_min, h_max, h_min, multi);
        return temp;
}

// Generuje losową mapę
//      przyjmuje strukturę mapy w której dane będą zapisywane, drugim parametrem jest liczba
//      całkowita "multi" - definije ona minimalną oraz maksymalną ilość szczytów lub dołów
//      nawet podając 0 za multi dalej minimalna ilość zagłębień to 5 aby nie było braków w mapie.
//
//      Generator ten działa następująco:
//              - losuje ilosć szczytów lub dołów
//              - dla każdego szczytu lub dołu:
//                      - losuje promień zniekrztałcenia terenu
//                      - maksymalną wartość niekrztałcenia
//                      - oraz index komórki która będzie szczytem lub dołem
//              - następnie zapisuje wszystkie komórki należące go tego obrębu
//              - potem wartości depth wybranych komórek są odpowiednio modyfikowane
void Map_Generate(Map* map) {
        srand(time(NULL));

        for (int i = 0; i < map->multi; i++) {
                Vector* dists = Vector_Construct(0);
                Vector* indexs = Vector_Construct(0);

                int index = rand() % map->depth->size;
                int radius = map->min_r + (rand() % (map->max_r - map->min_r));
                double max = map->min_h + (rand() % (map->max_h - map->min_h));
                if (rand() % 2) max *= - 1;

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

                Vector_Destroy(dists);
                Vector_Destroy(indexs);
        }
}

Map* Map_Load(const char* file_path) {
        FILE* save = fopen(file_path, "r");
        Map* temp = malloc(sizeof(Map));

        if (save == NULL) {
                printf("Błąd przy czytaniu mapy z pliku!\n");
                return NULL;
        }

        int counter = 0;
        double buf = 0;

        fscanf(save, "%d", &temp->width);
        fscanf(save, "%d", &temp->heigth);
        temp->depth = Vector_Construct(temp->width * temp->heigth);

        while ( fscanf(save, "%lf", &buf) == 1) {
                temp->depth->value[counter] = buf;
                counter++;
        }

        return temp;
}

// Oblicza odległość dwóch komórek w mapie o podanych indeksach
//      Pitagoras =)
double Map_GetDist(Map* map, int index0, int index1) {
        int x0 = index0 % map->width;
        int x1 = index1 % map->width;        
        double y0 = (index0 - x0)/ map->width;
        double y1 = (index1 - x1) / map->width;

        double x = fabs((double)(x0 - x1));
        double y = fabs(y0 - y1);

        return sqrt(x*x + y*y);
}

// Wyświetla wszystkie wartości komórek
void Map_Print(Map* map) {
        for (int i = 0; i < map->depth->size; i++) {
                printf("%3.1f ", map->depth->value[i]);

                if ( (i + 1) % map->width == 0 ) printf("\n");
        }
}


void draw_text(SDL_Renderer* renderer, TTF_Font* font,
        const char* text,
        int x, int y,
        SDL_Color color) {

        SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
        SDL_Rect rect = {x, y, surface->w, surface->h};
    
        SDL_RenderCopy(renderer, texture, NULL, &rect);
   
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
}


// Wizualizuje mapę za pomocą biblioteki SDL2 (Simple DirectMedia Layer)
//      tworzy okno o szerokości i wysokości odpowiadającej wielkości mapy podanej w parametrach.
//      Deklaruje i definiuje wszyzstkie niezbędne zmienne dla SDL2 (window, renderer, event)
//      aby wygodnie korzystać z programu.
//      Ważniejsze części kodu wytłumaczone są wewnątrz funkcji.
void Map_Visualize(Map* map) {
        SDL_Window* window = 
                SDL_CreateWindow("PSO", 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                MAP_SIZE + 180, MAP_SIZE + 140, 
                SDL_WINDOW_RESIZABLE);
        
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Event event;
        int running = 1;

        TTF_Init();
        TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);

        
        if (!font) {
                printf("Błąd ładowania czcionki: %s\n", TTF_GetError());
                return;
        }
        

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        double amp_p = map->depth->value[Vector_FindMax(map->depth)];
        double amp_d = (-1 * map->depth->value[Vector_FindMin(map->depth)]);
        
        // NOWE RYSOWANIE
        SDL_Texture* texture = SDL_CreateTexture(renderer,
                        SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING,
                        map->width, map->heigth);

        Uint32* pixels;
        int width;
        SDL_LockTexture(texture, NULL, (void**)&pixels, &width);

        //Rysowanie mapy
        //      dla każdego pixela/komórki tablicy mamy wylosowany depth->value no i zgodnie z legendą
        //      przyjmujemy kolejno od czerwieni dla wysoko położonych przez zieleń do niebieskiego dla
        //      najniżej położonych punktów. LEGENDA ZMIAN KOLORU
        for (int i = 0; i < map->depth->size; i++) {
                double depth = map->depth->value[i];    // aktualna głębokość/wysokość punktu
                
                int r;          //kolory rgb
                int g;
                int b;

                if (depth > 0) { // gdy punkt znajduję się nad poziomem 0      
                        r = 175 + (depth / amp_p) * 80;
                        g = 150 - (depth / amp_p) * 30;
                        b = 0;
               } else if (depth < 0) { // gdy punkt znajduję się pod poziomem zero
                        depth *= -1;
                        r = 175 - (depth / amp_d) * 80;
                        g = 150 - (depth / amp_d) * 30;
                        b = (depth / amp_d) * 40;
                } else { // zero
                        r = 175;
                        b = 0;
                        g = 150;
                }                      

                // jezeli wartość przekracza maksymalną to ustawiamy po prostu maks dla RGB
                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                //NOWY RYSOWANIE

                pixels[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }

        SDL_UnlockTexture(texture);
        
        SDL_Rect dstrect = {70, 70, MAP_SIZE, MAP_SIZE};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        
        //Linie mapy / numerkiii
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Color black = {10, 10, 10, 0};
        char buffer[20];

        for (int i = 0; i < map->heigth; i += map->heigth/10) {
                int h = MAP_SIZE + 70 - (((double)i / (double)map->heigth) * MAP_SIZE);
                sprintf(buffer, "%d", i);
                draw_text(renderer, font, buffer, 25, h - 15, black);
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
                SDL_RenderDrawLine(renderer, 70, h, 70 + MAP_SIZE, h);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawLine(renderer, 65, h, 70, h);
        }

        for (int i = 0; i < map->width; i += map->width/10) {
                int w = 70 + (((double)i / (double)map->width) * MAP_SIZE);
                sprintf(buffer, "%d", i);
                draw_text(renderer, font, buffer, w, MAP_SIZE + 70, black);
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
                SDL_RenderDrawLine(renderer, w, 70, w, MAP_SIZE + 70);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawLine(renderer, w, MAP_SIZE + 70, w, MAP_SIZE + 75);
        }

        int index_min = Vector_FindMin(map->depth);
        int index_max = Vector_FindMax(map->depth);
        double max = map->depth->value[index_max];
        double min = map->depth->value[index_min];

        //Rysowanie legendy
        for (int i = 0; i < MAP_SIZE; i++) {
                int r;    // 255 -> 175 -> 95   // legenda zmian kolorystyki z góry do dołu
                int g;    // 150 -> 120 -> 150
                int b;    // 0   -> 0   -> 40
                          
                if (i < (MAP_SIZE / 2)) {
                        double normalize = ((double)i / (double)(MAP_SIZE / 2));
                        r = 255 - (80 * normalize);
                        g = 120 + (30 * normalize);
                        b = 0;
                } else {
                        double normalize = ((double)(i - MAP_SIZE / 2) / (double)(MAP_SIZE / 2));
                        r = 175 - (80.0 * normalize);
                        g = 150 + (30.0 * normalize);
                        b = 40.0 * (normalize);
                }                        

                // jezeli wartość przekracza maksymalną to ustawiamy po prostu maks dla RGB
                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawLine(renderer, MAP_SIZE + 140, i + 70, MAP_SIZE + 180, i + 70);
        }

        double whole = max - min; 

        for (int i = 0; i <= MAP_SIZE / 100; i++) {
                int x = 135 + MAP_SIZE;
                int y = MAP_SIZE + 70 - (i * 100);

                sprintf(buffer, "%.1f", min + (whole * ((double)i / (double)(MAP_SIZE / 100))) );
                draw_text(renderer, font, buffer, x - 50, y, black);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
                SDL_RenderDrawLine(renderer, x, y, x + 45, y);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        //Peak
        {
                SDL_Rect dst;
                dst.w = 11; dst.h = 11;
                dst.x = 65 + (index_max % map->width) * (MAP_SIZE / map->width);
                dst.y = 65 + (index_max / map->width) * (MAP_SIZE / map->heigth);
                dst.x += ( (MAP_SIZE / map->width) / 2);
                dst.y += ( (MAP_SIZE / map->heigth) / 2);
                
                SDL_RenderFillRect(renderer, &dst);

                sprintf(buffer, "%.1f", map->depth->value[index_max]);
                draw_text(renderer, font, buffer, dst.x - 30, dst.y + 10, black);
        }

        //Lowest
        {
                SDL_Rect dst;
                dst.w = 11; dst.h = 11;
                dst.x = 65 + (index_min % map->width) * (MAP_SIZE / map->width);
                dst.y = 65 + (index_min / map->width) * (MAP_SIZE / map->heigth);
                dst.x += ( (MAP_SIZE / map->width) / 2);
                dst.y += ( (MAP_SIZE / map->heigth) / 2);
                
                SDL_RenderFillRect(renderer, &dst);
                
                sprintf(buffer, "%.1f", map->depth->value[index_min]);
                draw_text(renderer, font, buffer, dst.x - 30, dst.y + 10, black);
        }

        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        
        // Główna pętla, można sobie popatrzeć
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
                SDL_Delay(50);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
}

void Map_Destroy(Map* map) {
        Vector_Destroy(map->depth);
        free(map);
}

void Map_Save(Map* map) {
        FILE* save = fopen("save", "w");

        if ( save == NULL ) {
                printf("Błąd przy zapisie mapy!\n");
                return;
        }

        fprintf(save, "%d %d\n", map->width, map->heigth);

        for (int i = 0; i < map->depth->size; i++) {
                fprintf(save, " %.3lf", map->depth->value[i]);
                if (!((i + 1) % map->width)) fprintf(save, "\n");
        }

        fclose(save);
}
