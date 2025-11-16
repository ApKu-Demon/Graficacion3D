#pragma once

#include "../math/vectores.h"
#include "../color/colores.h"
#include "textura.h"

typedef struct caras_indices {
	int a, b, c;
    TexturaUV a_uv;
    TexturaUV b_uv;
    TexturaUV c_uv;
}Cara_t;

typedef enum _tipos_fig{
    LINEA = 2 << 0,
    CURVA = 2 << 1,
    CUADRO = 2 << 2,
    TRIAN = 2 << 3,
    CIRC = 2 << 4
}TypeFig;

typedef struct  _cuadro{
    Vec2 pos;
    int w;
    int h;
    char offset_mem1[8];
    Color color;
    TypeFig type;
}Cuadro;

typedef struct  _circulo{
    Vec2 pos;
    float r;
    int vert;
    char offset_mem1[8];
    Color color;
    TypeFig type;
}Circulo;

typedef struct _triangulo{
    Vec3 pos[3];
    Color color;
    TypeFig type;
    float avg_z;
    Vec3 normal;
    TexturaUV texuv[3];
}Triangulo;

typedef struct _linea{
    Vec2 p1;
    Vec2 p2;
    char offset_mem1[8];
    Color color;
    TypeFig type;
}Linea;

typedef struct _curva{
    Vec2 p1;
    Vec2 p2;
    Vec2 p3;
    Color color;
    TypeFig type;
}Curva;

typedef struct _figdummy{
    char offset[28];
    TypeFig type;
}FigComun;

typedef union  _figuras{
    FigComun data;
    Cuadro cuadro;
    Circulo circulo;
    Triangulo triangulo;
    Linea linea;
    Curva curva;
}Figuras;

void draw_figura(Figuras *fig);
Vec3 normal_triangulo(Triangulo *trian);
