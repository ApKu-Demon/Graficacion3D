#pragma once

#include <stdint.h>

typedef union _color {
    uint32_t hex;
    struct{
        uint8_t a;
        uint8_t b;
        uint8_t g;
        uint8_t r;
    }rgba;
}Color;

Color lerp_color(Color color_start, Color color_end, float steps);

uint32_t *cargar_imagen(const char *path, int *x, int *y, int *c, int n);