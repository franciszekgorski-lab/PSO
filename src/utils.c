#include "utils.h"
#include <stdlib.h>

// Tworzy strukturę
Vector* Vector_Construct(int s) {
        Vector* temp = malloc(sizeof(Vector));
        temp->size = s;
        if (s != 0)     temp->value = malloc(s * sizeof(double));
        else            temp->value = NULL;

        return temp;
}

// Dodaje element na koniec tablicy
void Vector_Append(Vector* v, double value) {
        if (v == NULL) return;

        v->size++;

        if (v->value != NULL) v->value = realloc(v->value, v->size * sizeof(double));
        else v->value = malloc(v->size * sizeof(double));

        v->value[v->size - 1] = value;
}

// Zmienia wielkość tablicy
//      jeżeli nowa wielkość jest mniejsza niż poprzednia to tracone są stare elementy ;(
//      gdy nowa wielkość jest większa to nadaje nowym elementom wartość 0
//      zwraca starą wielkość
int Vector_ChangeSize(Vector* v, int s) {
        if (v == NULL) return -1;

        int temp = v->size;
        v->value = realloc(v->value, s * sizeof(double));
        v->size = s;
        return temp;
}

// Wyrzuca element z końca tablicy
//      zwraca wyrzucony element
double Vector_Pop(Vector* v) {
        if (v == NULL) return 1;

        if (v->size > 0) {
                double temp = v->value[v->size - 1];

                v->size--;
                v->value = realloc(v->value, v->size * sizeof(double));
                
                return temp;
        } else return 1;
}

//Usuwanie odpowiedniego elementu
double Vector_Delete(Vector* v, int index) {
        if (v == NULL) return 1;

        v->size--;
        double temp = v->value[index] = v->value[v->size];
        v->value = realloc(v->value, v->size * sizeof(double));

        return temp;
}

//Szukanie największej wartości w tablicy
//      zwraca jej indeks
int Vector_FindMax(Vector* v) {
        int max = 0;

        for (int i = 0; i < v->size; i++) {
                if (v->value[max] < v->value[i]) max = i;
        }

        return max;
}

//Szukanie najmniejszej wartości w tablicy
//      zwraca jej indeks
int Vector_FindMin(Vector* v) {
        int min = 0;

        for (int i = 0; i < v->size; i++) {
                if (v->value[min] > v->value[i]) min = i;
        }

        return min;
}

void Vector_Destroy(Vector* v) {
        if (v == NULL) return;
        if (v->value != NULL) free(v->value);
        free(v);
}
