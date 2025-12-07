#pragma once

#include "../math/vectores.h"
#include "linea.h"
#include "figuras.h"

#include <stdint.h>

void draw_pixel(int x, int y, uint32_t color);
void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_cuadrado(int x0, int y0, int largo, int altura, uint32_t color);
void draw_circuloTrig(int radio , int x0, int y0, int numvertices, uint32_t color);
void draw_circuloPM(int radio , int x0, int y0, uint32_t color);
void draw_linea(int x0, int y0, int x1, int y1, uint32_t color);

// Curvas
void draw_linea_lerp(Vec2 p1, Vec2 p2, uint32_t color);
void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color);

// Rellenonos
void fill_triangulo(Triangulo *triangulo, uint32_t color);
void fill_figura(void *figura, uint32_t fill_color);
void fill_flood(Vec2 *pInicial, uint32_t fill_color);
void fill_cuadro(Vec3 pos, int w, int h, uint32_t color, uint32_t contorno);

// Relleno triangulo nuevo
// Relleno triangulo
void fill_trian(Vec4 p1, Vec4 p2, Vec4 p3, uint32_t color);
void draw_trian_pixel(int x, int y, uint32_t color, Vec4 p1, Vec4 p2, Vec4 p3);

uint32_t getColor(Vec2 *pixel);

// textura
Vec3 barycentric_pesos(Vec3 a, Vec3 b, Vec3 c, Vec3 p);
void draw_textura(int x, int y, 
                Vec4 a, Vec4 b, Vec4 c, 
                TexturaUV t1, TexturaUV t2, TexturaUV t3, 
                uint32_t* textura, int tw, int th);
void tex_trian(	Vec4 p1, TexturaUV tuv1, 
                Vec4 p2, TexturaUV tuv2, 
                Vec4 p3, TexturaUV tuv3, 
                uint32_t *textura, int tw, int th);


