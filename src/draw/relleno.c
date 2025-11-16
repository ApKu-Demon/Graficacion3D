#include "draw.h"
#include "figuras.h"
#include "../global.h"
#include "../estructuras/pila.h"
#include "../memoria/memoria.h"
#include "stdio.h"

#include <stdlib.h>

void fill_flat_bottom(Vec3 p1,Vec3 p2, Vec3 p3, uint32_t color);
void fill_flat_top(Vec3 p1,Vec3 p2, Vec3 p3, uint32_t color);
void ordenar_y(Vec3 *y);
uint32_t getColor(Vec2 *pixel);

void fill_figura(void *figura, uint32_t fill_color){
    Figuras *figuraCast = (Figuras*)figura;
    switch(figuraCast -> data.type){
        case CIRC:{
            Circulo *circCast = (Circulo*)figuraCast;
            fill_flood(&(circCast -> pos), fill_color);
            break;}
        case CUADRO:{
            Cuadro *cuadCast = (Cuadro*)figuraCast;

            int xM = cuadCast -> pos.unpack.x + (cuadCast -> w / 2);
            int yM = cuadCast -> pos.unpack.y + (cuadCast -> h / 2);

            Vec2 puntoM = {{xM, yM}};
            puntoM.unpack.x = xM;
            puntoM.unpack.y = yM;

            fill_flood(&puntoM, fill_color);
            break;}
        case TRIAN:{
            Triangulo *trianCast = (Triangulo*)figuraCast;
            fill_triangulo(trianCast, fill_color);
            break;}
        case LINEA:
            break;
        case CURVA:
            break;
    }
}

void fill_flood(Vec2 *pInicial, uint32_t fill_color){
    Pila *pila = (Pila*)calloc(1, sizeof(Pila));

    int total_pixeles = estadosrender.w_height * estadosrender.w_width;
    char *verificado = (char*)calloc(total_pixeles, sizeof(char));

    Vec2 *pixel = malloc(sizeof(Vec2));
    pixel->unpack.x = pInicial->unpack.x;
    pixel->unpack.y = pInicial->unpack.y;

    push(pila, pixel);

    uint32_t colorObj = getColor(pixel);

    while(pila -> cima != NULL){
        pixel = (Vec2*)pop(pila);
        int x = pixel -> unpack.x;
        int y = pixel -> unpack.y;

        int idx = y * estadosrender.w_width + x;

        if(x >= 0 && x < estadosrender.w_width &&
           y >= 0 && y < estadosrender.w_height &&
           verificado[idx] == 0 &&
           getColor(pixel) == colorObj)
        {
            draw_pixel(x, y, fill_color);
            verificado[idx] = 1;

            Vec2 vecinos[4] = {{{1, 0}},
                               {{-1, 0}},
                               {{0, 1}},
                               {{0, -1}}};

            for(int i = 0; i < 4; i++){
                int newX = x + vecinos[i].unpack.x;
                int newY = y + vecinos[i].unpack.y;

                if(newX >= 0 && newX < estadosrender.w_width &&
                   newY >= 0 && newY < estadosrender.w_height)
                {
                    int pixelPos = newY * estadosrender.w_width + newX;
                    if(verificado[pixelPos] == 0){
                        Vec2 *objetivo = malloc(sizeof(Vec2));
                        objetivo->unpack.x = newX;
                        objetivo->unpack.y = newY;
                        push(pila, objetivo);
                    }
                }
            }
        }
        free(pixel);
    }

    free(verificado);
    freePila(pila);
    free(pila);
}

void fill_triangulo(Triangulo *triangulo, uint32_t color){
    // Ordenar
    float cy, cx;
    Vec3 y[3];

    y[0] = triangulo -> pos[0];
    y[1] = triangulo -> pos[1];
    y[2] = triangulo -> pos[2];
    ordenar_y(y);

    // Pintar
    if (y[1].unpack.y == y[2].unpack.y) {
        fill_flat_bottom(y[0], y[1], y[2], color);
    } else if (y[0].unpack.y == y[1].unpack.y) {
        fill_flat_top(y[0], y[1], y[2], color);
    } else {
        cy = y[1].unpack.y;
        cx = (y[1].unpack.y - y[0].unpack.y) * (y[2].unpack.x - y[0].unpack.x) / (y[2].unpack.y - y[0].unpack.y) + y[0].unpack.x;

        Vec3 v = {{cx, cy}};
        fill_flat_bottom(y[0], y[1], v, color);
        fill_flat_top(v, y[1], y[2], color);
    }
}

void ordenar_y(Vec3 *y){
    Vec3 aux;

    for(int i = 0; i < 3; i++){
        for(int j = i + 1; j < 3; j++){
            if(y[i].unpack.y > y[j].unpack.y){
                aux = y[i];
                y[i] = y[j];
                y[j] = aux;
            }
        }
    }
}

void fill_flat_bottom(Vec3 p2, Vec3 p3, Vec3 c, uint32_t color){
    float mi = (p3.unpack.x - p2.unpack.x) / (p3.unpack.y - p2.unpack.y);
    float mf = (c.unpack.x - p2.unpack.x) / (c.unpack.y - p2.unpack.y);

    float xi = p2.unpack.x;
    float xf = p2.unpack.x;

    for (int y = (int)p2.unpack.y; y <= (int)c.unpack.y; y++) {
        draw_linea(xi, y, xf, y, color);
        xi += mi;
        xf += mf;
    }
}

void fill_flat_top(Vec3 p1, Vec3 p2 ,Vec3 c, uint32_t color){
    float mi = (c.unpack.x - p1.unpack.x) / (c.unpack.y - p1.unpack.y);
    float mf = (c.unpack.x - p2.unpack.x) / (c.unpack.y - p2.unpack.y);

    float xi = c.unpack.x;
    float xf = c.unpack.x;

    for (int y = (int)c.unpack.y; y >= (int)p1.unpack.y; y--) {
        draw_linea(xi, y, xf, y, color);
        xi -= mi;
        xf -= mf;
    }
}

uint32_t getColor(Vec2 *pixel){
    return estadosrender.color_buffer[(int)pixel->unpack.y * estadosrender.w_width + (int)pixel->unpack.x];
}
