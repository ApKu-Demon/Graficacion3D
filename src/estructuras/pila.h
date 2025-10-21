#pragma once

#include "nodo.h"

typedef struct pila
{
    Nodo *cima;
    int cantidad;
}Pila;

void push(Pila *pila, void *dato);
void* pop(Pila *pila);


