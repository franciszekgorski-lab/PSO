#include "map.h"
#include "utils.h"

#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Inicjalizacja struktury Mapy,
//      zawiera ona wszystko potrzebne do stworzenia mapy "trzy wymiarowej" z rzutem z góry.
//      Implementacją tej "mapy" jest tablica zmiennych double, w każdej komórce tablicy
//      znajdziesz wartość depth (ang. głebokość) co odnosi do się ogólnego położenia
//      punktu względem poziomu zerowego, nazwa depth nie jest więc zbyt domyślna,
//      ale taka już jest.
Map* Map_Construct(int w, int h, int max_d) {
        Map* temp = malloc(sizeof(Map));
        temp->width = w;
        temp->heigth = h;
        temp->max_depth = max_d;
        temp->depth = Vector_Construct(w * h);

        for (int i = 0; i < temp->depth->size; i++)      temp->depth->value[i] = 0;
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
void Map_Generate(Map* map, int multi) {
        srand(time(NULL));

        int peak_count = 5 + multi + (rand() % (10 + multi));

        for (int i = 0; i < peak_count; i++) {
                Vector* dists = Vector_Construct(0);
                Vector* indexs = Vector_Construct(0);

                int index = rand() % map->depth->size;
                int radius = 30 + (rand() % map->max_depth);
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
/*
void draw_text(SDL_Renderer* renderer,
        TTF_Font* font, const char* text,
        int x, int y,
        SDL_Color color) {

        SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
        SDL_Rect rect = {x, y, surface->w, surface->h};
    
        SDL_RenderCopy(renderer, texture, NULL, &rect);
   
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
}
*/
// Wizualizuje mapę za pomocą biblioteki SDL2 (Simple DirectMedia Layer)
//      tworzy okno o szerokości i wysokości odpowiadającej wielkości mapy podanej w parametrach.
//      Deklaruje i definiuje wszyzstkie niezbędne zmienne dla SDL2 (window, renderer, event)
//      aby wygodnie korzystać z programu.
//      Ważniejsze części kodu wytłumaczone są wewnątrz funkcji.
void Map_Visualize(Map* map) {
        SDL_Window* window = 
                SDL_CreateWindow("PSO", 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                map->width + 180, map->heigth + 140, 
                SDL_WINDOW_RESIZABLE);
        
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Event  event;
        int running = 1;

        //TTF_Init();
        //TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);

        /*
        if (!font) {
                printf("Błąd ładowania czcionki: %s\n", TTF_GetError());
                return;
        }
        */

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

                if (depth > 0) { // gdy punkt znajduję się nad poziomem 0      <LEGENDA ZMIAN KOLORU
                        r = 175 + (depth / amp_p) * 80;
                        g = 160 - (depth / amp_p) * 30;
                        b = 0;
                } else if (depth < 0) { // gdy punkt znajduję się pod poziomem zero
                        depth *= -1;
                        r = 170 - (depth / amp_d) * 80;
                        g = 160 - (depth / amp_d) * 30;
                        b = (depth / amp_d) * 40;
                } else { // zero
                        r = 175;
                        b = 0;
                        g = 160;
                }                                       //                      LEGENDA ZMIAN KOLORU>

                // jezeli wartość przekracza maksymalną to ustawiamy po prostu maks dla RGB
                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                //NOWY RYSOWANIE

                pixels[i] = (0xFF << 24) | (r << 16) | (g << 8) | b;
                
                //STARE RYSOWANIE
                //SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                //SDL_RenderDrawPoint(renderer, (i % map->width) + 80, i / map->width);
        }

        SDL_UnlockTexture(texture);
        
        SDL_Rect dstrect = {70, 70, map->width, map->heigth};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        //Linie mapy / numerkiii
        //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        //SDL_Color black = {255, 0, 0, 0};
        //char buffer[20];

        for (int i = 0; i <= map->heigth / 100; i++) {
                //sprintf()
                //draw_text(renderer, font, buffer, 30, 70 + (i * 100), black);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
                SDL_RenderDrawLine(renderer, 70, 70 + (i * 100), 70 + map->width, 70 + (i * 100));
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                SDL_RenderDrawLine(renderer, 65, 70 + i * 100, 70, 70 + i * 100);
        }

        for (int i = 0; i <= ((map->width / 100) + 0); i++) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
                SDL_RenderDrawLine(renderer, 70 + i * 100, 70, 70 + i * 100, map->heigth + 70);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                SDL_RenderDrawLine(renderer, 70 + i * 100, map->heigth + 70, 70 + i * 100, map->heigth + 75);
        }

        //Rysowanie legendy
        for (int i = 0; i < map->heigth; i++) {
                int r;    // 255 -> 175 -> 95   // legenda zmian kolorystyki z góry do dołu
                int g;    // 160 -> 130 -> 160
                int b;    // 0   -> 0   -> 40
                          
                if (i < (map->heigth / 2)) {
                        double normalize = ((double)i / (double)(map->heigth / 2));
                        r = 255 - (80 * normalize);
                        g = 160 - (30 * normalize);
                        b = 0;
                } else {
                        double normalize = ((double)(i - map->heigth / 2) / (double)(map->heigth / 2));
                        r = 175 - (80.0 * normalize);
                        g = 130 + (30.0 * normalize);
                        b = 0 + 40.0 * (normalize);
                }

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawLine(renderer, map->width + 140, i, map->width + 180, i);
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
}
