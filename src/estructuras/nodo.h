#pragma once

#include "../math/vectores.h"

typedef struct _nodo{
    void *dato;
    struct _nodo *sig;
}Nodo;

Nodo* crearNodo(void *vector);