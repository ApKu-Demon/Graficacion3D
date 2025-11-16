#include "nodo.h"
#include "../math/vectores.h"

#include <stdlib.h>

Nodo* crearNodo(void *vector){
    Nodo *aux = (Nodo*)calloc(1, sizeof(Nodo));
    aux -> dato = vector;
    aux -> sig = NULL;

    return aux;
}