#pragma once

#include <stdint.h>

void draw_linea(int x0, int y0, int x1, int y1, uint32_t color);

void fuerza_bruta(int x0, int y0, int x1, int y1, uint32_t color);

void linea_dda(int x0, int y0, int x1, int y1, uint32_t color);

void linea_brhm(int x0, int y0, int x1, int y1, uint32_t color);
