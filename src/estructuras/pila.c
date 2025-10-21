#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

void push(Pila *pila, void *dato)
{
    Nodo *nuevo = crearNodo(dato);
	nuevo->sig = pila->cima;
    pila->cima = nuevo;
    pila->cantidad++;
}

void* pop(Pila *pila)
{
    if(!pila)
	{
		printf("\n Underflow");
		return NULL;
	}

    Nodo *aux = pila->cima;
    pila->cima = aux->sig;
    aux->sig = NULL;
    void *dato = aux->dato;
    free(aux);
    pila->cantidad--;
    return dato;
}

