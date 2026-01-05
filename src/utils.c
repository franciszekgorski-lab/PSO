#include "utils.h"
#include <stdlib.h>

Vector* Vector_Construct(int s) {
        Vector* temp = malloc(sizeof(Vector));
        temp->size = s;
        temp->value = malloc(s * sizeof(int));

        return temp;
}

void Vector_Append(Vector* v, int value) {
        if (v == NULL) return;

        v->size++;
        v->value = realloc(v->value, v->size * sizeof(int));
        v->value[v->size - 1] = value;
}

int Vector_ChangeSize(Vector* v, int s) {
        if (v == NULL) return -1;

        int temp = v->size;
        v->value = realloc(v->value, s * sizeof(int));
        v->size = s;
        return temp;
}

int Vector_Pop(Vector* v) {
        if (v == NULL) return 1;

        if (v->size > 0) {
                int temp = v->value[v->size - 1];

                v->size--;
                v->value = realloc(v->value, v->size * sizeof(int));
                
                return temp;
        } else return 1;
}

int Vector_Delete(Vector* v, int index) {
        if (v == NULL) return 1;

        v->size--;
        int temp = v->value[index] = v->value[v->size];
        v->value = realloc(v->value, v->size * sizeof(int));

        return temp;
}
