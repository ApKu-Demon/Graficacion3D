#pragma once

#define pushto_array(array, valor)                               \
    do{                                                          \
        (array) = array_expander((array), 1, sizeof(*(array)));  \
        (array)[array_size(array) - 1] = (valor);                \
    }while(0);

void* array_expander(void *array, int num, int size_dato);
int array_size(void *array);
void free_array(void *array);