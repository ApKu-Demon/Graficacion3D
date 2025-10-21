#pragma once

typedef struct nodo
{
    void *dato;
    struct nodo *sig;
}Nodo;

Nodo* crearNodo(void* dato);


