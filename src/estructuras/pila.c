#include "pila.h"
#include "../math/vectores.h"

#include <stdio.h>
#include <stdlib.h>

void push(Pila *pila, void *vector){
    Nodo *aux = crearNodo(vector);
    
    aux -> sig = pila -> cima;
    pila -> cima = aux;
    pila -> datos++;
}

void* pop(Pila *pila){
    if(pila){
        Nodo *aux = pila -> cima;

		pila -> cima = aux -> sig;
		aux -> sig = NULL;
		void *dato = aux -> dato;

		free(aux);
		pila -> datos--;

		return dato;
    }
    return NULL;
}

void freePila(Pila *pila){
    Nodo *aux;

    while (pila && pila->cima != NULL) {
        aux = pila -> cima;
        pila -> cima = aux -> sig;
        
        free(aux);
    }
}