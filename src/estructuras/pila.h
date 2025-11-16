#pragma once

#include "nodo.h"

typedef struct  _pila{
    Nodo *cima;
    int datos;
}Pila;

void* pop(Pila *pila);
void push(Pila *pila, void *vector);
void freePila(Pila *pila);
