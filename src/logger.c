#include "logger.h"

//wypisuje gorny rzad pliku csv z nazwami kolumn
void log_headers(FILE * fptr, swarm * s){
        //1 kolumna to numer - ktory mowi nam ile razy pozycje byly dotad zloggowane
        fprintf(fptr, "numer log,");
        for(int j=0; j<s->size; j++){
                //nastepne 3 kolumny dla każdej komorki to jej numer, i pozycja x i y
                fprintf(fptr, "nr.czastki,X,Y,");
        }
        fprintf(fptr, "\n");
}

//zapisuje pozycje do pliku csv w formacie numer linii(iteracja), numer czastki, poz x, poz y, itd po ostatniej czastce \n
void log_positions( FILE * fptr, swarm * s, int iteration){
    fprintf(fptr,"%d,",iteration);
    for(int i=0; i<s->size; i++){
        particle * p = s->particle_arr[i];
        fprintf(fptr,"%d,%lf,%lf,", i, p->pos_x, p->pos_y);
    }
    fprintf(fptr,"\n");
}

//zapisuje rezultaty tzn pozycje x i y (calkowita) najwyzszego znalezionego punktu
void log_results(FILE * fptr, swarm * s){
    fprintf(fptr,"%d,%d",  s->highest_found_x,s->highest_found_y);
}
