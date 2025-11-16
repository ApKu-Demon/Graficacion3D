#include "linea.h"
#include "draw.h"

#include <math.h>

#define LINEA_BRESSENHAM 1
#define ANTI_ALIASING 0

void draw_linea(int x0, int y0, int x1, int y1, uint32_t color){
    #if LINEA_BRUTA
        fuerza_bruta(x0, y0, x1, y1, color);
    #elif LINEA_DDA
        linea_dda(x0, y0, x1, y1, color);
    #elif LINEA_BRESSENHAM
        linea_brhm(x0, y0, x1, y1, color);

        #if ANTI_ALIASING
            Xiaolinline(x0, y0, x1, y1, color);
        #endif
    #endif
}

void fuerza_bruta(int x0, int y0, int x1, int y1, uint32_t color){
    int y;
    float dx = fabs((float)(x1 - x0));
    float dy = fabs((float)(y1 - y0));
    
    for(int x = x0; x <= x1; ++x){
        y = y0 + dy * ((x - x0) / dx);
        draw_pixel(x, y, color);
    }
}

void linea_dda(int x0, int y0, int x1, int y1, uint32_t color){
    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int largo = (fabs(dx) > fabs(dy)) ? fabs(dx):fabs(dy);

    float inc_x = dx / (float)largo;
    float inc_y = dy / (float)largo;

    float x = x0;
    float y = y0;

    for(int i = 0; i < largo; i++){
        draw_pixel((int)x, (int)y, color);
        x += inc_x;
        y += inc_y;
    }
}

void linea_brhm(int x0, int y0, int x1, int y1, uint32_t color){
    int dx = fabs(x1 - x0);
    int sx = (x0 < x1) ? 1:-1;
    int dy = -fabs(y1 - y0);
    int sy = (y0 < y1) ? 1:-1;

    int error = dx + dy;
    int x = x0;
    int y = y0;

    while(1){
        draw_pixel(x, y, color);
	int e2 = 2 * error;

        if(x == x1 && y == y1) break;

        if(e2 >= dy){
            error = error + dy;
            x += sx;
        }
        if(e2 <= dx){
            error = error + dx;
            y += sy;
        }
    }
}

void Xiaolinline(int x0, int y0, int x1, int y1, uint32_t color){
    if(fabs(x1 - x0) > fabs( y1- y0)){
        if(x1 < x0){
            l_swap(&x0, &x1);
            l_swap(&y0, &y1);
        }

        float dx = (float)(x1 - x0);
        float dy = (float)(y1 - y0);
        float m = (dx != 0) ? (dy / dx):(0.f);

        float overlap = rfpart(x0);
        float distinicio = rfpart(y0 + 0.5);

        uint32_t c1 = cambiar_brillo(&color, (1 - distinicio) * overlap);
        uint32_t c2 = cambiar_brillo(&color, distinicio * overlap);

        draw_pixel(x0 + 0.5, y0, c1);
        draw_pixel(x0 + 0.5, y0 + 1, c2);

        for(float i = 1; i < round(dx + 0.5); i++){
            float y = y0 + i * m;
            int ix = (x0 + i);
            int iy = (int)y;
            float dist = fpart(y + 0.5);

            c1 = cambiar_brillo(&color, 1 - dist);
            c2 = cambiar_brillo(&color, dist);

            draw_pixel(ix, iy, c1);
            draw_pixel(ix, iy + 1, c2);
        }
    }else{
        if(y1 < y0){
            l_swap(&x0, &x1);
            l_swap(&y0, &y1);
        }

        float dx = (float)(x1 -x0);
        float dy = (float)(y1 - y0);
        float m = (dy != 0) ? (dx / dy):(0.f);
        float overlap = rfpart(y0);
        float distinicio = fpart(y0 + 0.5);

        uint32_t c1 = cambiar_brillo(&color, (1 - distinicio) * overlap);
        uint32_t c2 = cambiar_brillo(&color, distinicio * overlap);

        draw_pixel(x0 + 0.5, y0, c1);
        draw_pixel(x0 + 0.5, y0 + 1, c2);

        overlap = ((y1 - 0.5) - (int)(y1 - 0.5));
        float distfin = fpart(y1 + 0.5);

        c1 = cambiar_brillo(&color, (1 - distfin) * overlap);
        c2 = cambiar_brillo(&color, distfin * overlap);

        draw_pixel(x1, y1 + 0.5, c1);
        draw_pixel(x1 + 1, y1 + 0.5, c2);

        for(float i = 1; i < round(dy + 0.5); i++){
            float x = x0 + i * m;
            int ix = (int)(x);
            int iy = (y0 + 1);
            float dist = fpart(x + 0.5);

            c1 = cambiar_brillo(&color, 1 - dist);
            c2 = cambiar_brillo(&color, dist);

            draw_pixel(ix, iy, c1);
            draw_pixel(ix + 1, iy, c2);
        }
    }
}

uint32_t cambiar_brillo(uint32_t *color, float brillo){
    if(brillo > 1.f){
        brillo = 1.f;
    }

    if(brillo <= 0.f){
        brillo = 0.f;
    }

    uint8_t rojo = (*color & 0xFF000000) >> 24;
    uint8_t verde = (*color & 0x00FF0000) >> 16;
    uint8_t azul = (*color & 0x0000FF00) >> 8;
    uint8_t alpha = (*color & 0x000000FF) >> 0;

    rojo = (uint8_t)(rojo * brillo);
    verde = (uint8_t)(verde * brillo);
    azul = (uint8_t)(azul * brillo);

    return (rojo << 24) | (verde << 16) | (azul << 8) | ( alpha << 0);
}

float fpart(float x){
    return x - floor(x);
}

float rfpart(float x){
    return 1 - fpart(x);
}

void l_swap(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}
//make DVAR=-DLINEA_XXXXXXXXX && make run
