#include "draw.h"
#include "linea.h"
#include <math.h>

#define PI 3.14159265358979323846

void draw_circuloTrig(int radio, int x0, int y0, int numvertices, uint32_t color){
    for(int i = 0; i < numvertices; ++i){
        float angulo = (i * 2 * PI) / numvertices;
        float angulo_s = (((i + 1) % numvertices) * 2 * PI) / numvertices;

        int xa = x0 + cos(angulo) * radio;
        int ya = y0 + sin(angulo) * radio;
        int xs = x0 + cos(angulo_s) * radio;
        int ys = x0 + sin(angulo_s) * radio;

        draw_linea(xa, ya, xs, ys, color);
    }
}

void draw_circuloPM(int radio , int x0 , int y0, uint32_t color){
    int x = 0;
    int y = radio;
    int p = 1 - radio;

    while(x <= y){
        draw_pixel(x0 + x, y0 + y, color);
        draw_pixel(x0 - x, y0 + y, color);
        draw_pixel(x0 + x, y0 - y, color);
        draw_pixel(x0 - x, y0 - y, color);
        draw_pixel(x0 + y, y0 + x, color);
        draw_pixel(x0 - y, y0 + x, color);
        draw_pixel(x0 + y, y0 - x, color);
        draw_pixel(x0 - y, y0 - x, color);
        x++;

        if(p < 0){
            p += 2 * x + 1;
        }else{
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}