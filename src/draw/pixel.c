#include "draw.h"
#include "../global.h"

void draw_pixel(int x, int y, uint32_t color){
    if(x < 0 || y < 0) return;
    if(x >= estadosrender.w_width || y >= estadosrender.w_height) return;

    estadosrender.color_buffer[y * estadosrender.w_width + x] = color;
}