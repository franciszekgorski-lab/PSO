#include "logger.h"

//zapisuje pozycje do pliku csv w formacie numer linii(iteracja), numer czastki, poz x, poz y, itd po ostatniej czastce \n
void log_positions( FILE * fptr, swarm * s, int iteration ){
    fprintf(fptr,"%d",iteration);
    for(int i=0; i<s->size; s++){
        particle p = s->particle_arr[i];
        fprintf(fpt,"%d, %lf, %lf, ", i, p->pos_x, p->pos_y);
    }
    fprintf(fpt,"\n");
}