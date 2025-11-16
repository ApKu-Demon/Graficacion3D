#include "memoria.h"
#include <stdlib.h>
#include <string.h>

#define ARRAY_DATA(array) ((int*)(array) - 2)
#define ARRAY_CAPACIDAD(array) (ARRAY_DATA(array)[0])
#define ARRAY_SIZE(array) (ARRAY_DATA(array)[1])

void* array_expander(void *array, int num, int size_dato){
    if(!array){
        int raw_size = (sizeof(int) * 2 ) + (size_dato * num);
        int *base = (int*)malloc(raw_size);

        memset(base, 0 , raw_size);

        base[0] = num;  // Capacidad
        base[1] = num;    // Size
        return base + 2;
    } else if (ARRAY_SIZE(array) + num <= ARRAY_CAPACIDAD(array)){
        ARRAY_SIZE(array) += num;
        return array;
    } else {
        // No hay espacio, hay que hacer mas
        int faltante = ARRAY_SIZE(array) + num;
        int doble_cap = ARRAY_CAPACIDAD(array) * 2;
        int capacidad = (faltante > doble_cap) ? faltante:doble_cap;
        int size = faltante;
        int raw_size = sizeof(int) * 2 + size_dato * capacidad;
        int *base = (int*)realloc(ARRAY_DATA(array), raw_size);
        base[0] = capacidad;
        base[1] = size;
        return base + 2;
    }
}

int array_size(void *array){
    return (array) ? ARRAY_SIZE(array):0;
}

void free_array(void *array){
    if(array){
        free(ARRAY_DATA(array));
    }
}