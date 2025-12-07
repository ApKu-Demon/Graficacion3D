#include "colores.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define ARGB_TO_RGBA(r, g, b, a)((uint32_t)((r<<24) | (g<<16) | (b<<8) | (a)))

Color lerp_color(Color a, Color b, float t){
    Color nuevo;

    nuevo.rgba.r = a.rgba.r + (b.rgba.r - a.rgba.r) * t;
    nuevo.rgba.g = a.rgba.g + (b.rgba.g - a.rgba.g) * t;
    nuevo.rgba.b = a.rgba.b + (b.rgba.b - a.rgba.b) * t;
    nuevo.rgba.a = 0xFF;

    return nuevo;
}

uint32_t *cargar_imagen(const char *path, int *x, int *y, int *c, int n) {
    unsigned char *data = stbi_load(path, x, y, c, n);

    if(!data) {
        fprintf(stderr, "Error al cargar la imagen\n");
        return 0;
    }

    uint32_t *img = (uint32_t*)data;

    // if(*c == 3 && n == 4) 
     {
        for(int i = 0; i < *x * *y; ++i) {
            uint8_t a = (img[i]& 0xff000000) >> 24;
            uint8_t b = (img[i]& 0x00ff0000) >> 16;
            uint8_t g = (img[i]& 0x0000ff00) >> 8;
            uint8_t r = (img[i]& 0x000000ff);             
            img[i] = ARGB_TO_RGBA(r, g, b, a);
        }
     }

    return img;
}