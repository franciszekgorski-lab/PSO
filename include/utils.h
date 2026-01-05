#ifndef UTILS
#define UTILS

//Uniwersalna lista liczb calkowitych,
//      możemy to robić dla każdej pojawiającej się listy,
//      ale myślę że wystarczy dać tu klika dobrych funkcji i będzie spokój
typedef struct _vector {
        int size;
        int* value;
} Vector;

Vector* Vector_Construct(int s);
void Vector_Append(Vector* v, int value);       
int Vector_ChangeSize(Vector* v, int s);        //zwraca poprzednią długość
int Vector_Pop(Vector* v);                      //zwraca usuniętą wartość
int Vector_Delete(Vector* v, int index);        //zwraca usuniętą wartość

#endif
