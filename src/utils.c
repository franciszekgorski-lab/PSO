#include "utils.h"
#include <stdlib.h>

Vector* Vector_Construct(int s) {
        Vector* temp = malloc(sizeof(Vector));
        temp->size = s;
        temp->value = malloc(s * sizeof(double));

        return temp;
}

void Vector_Append(Vector* v, double value) {
        if (v == NULL) return;

        v->size++;
        v->value = realloc(v->value, v->size * sizeof(double));
        v->value[v->size - 1] = value;
}

int Vector_ChangeSize(Vector* v, int s) {
        if (v == NULL) return -1;

        int temp = v->size;
        v->value = realloc(v->value, s * sizeof(double));
        v->size = s;
        return temp;
}

double Vector_Pop(Vector* v) {
        if (v == NULL) return 1;

        if (v->size > 0) {
                double temp = v->value[v->size - 1];

                v->size--;
                v->value = realloc(v->value, v->size * sizeof(double));
                
                return temp;
        } else return 1;
}

double Vector_Delete(Vector* v, int index) {
        if (v == NULL) return 1;

        v->size--;
        double temp = v->value[index] = v->value[v->size];
        v->value = realloc(v->value, v->size * sizeof(double));

        return temp;
}
