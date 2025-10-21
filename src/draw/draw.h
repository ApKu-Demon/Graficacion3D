#pragma once

#include <stdint.h>
#include "../math/vectores.h"
#include "linea.h"
#include "figuras.h"



void draw_pixel(int x, int y, uint32_t color);

uint32_t get_colorPixel(int x, int y);



// Figuras . . .

void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void draw_cuadrado(int x0, int y0, int largo, int altura, uint32_t color);	// sirve para hacer rectangulos tmb

void draw_circuloTrig(int radio, int x, int y, int numvertices, uint32_t color);

void draw_circuloPM(int radio, int xc, int yc, int borde, uint32_t color);

void draw_linea(int x0, int y0, int x1, int y1, uint32_t color);



// Curvas . . .

void draw_linea_lerp(Vec2 p1, Vec2 p2, uint32_t color);

void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color);



// Pintar . . .

void fill_trian(Triangulo triangulo);

