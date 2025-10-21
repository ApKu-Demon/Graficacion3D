#include "draw.h"
#include "../math/lerp.h"
#include "../math/vectores.h"

void draw_linea_lerp(Vec2 p1, Vec2 p2, uint32_t color) 
{
	Vec2 linea;
	for(float t=0; t<1; t+=0.001)
	{
		linea = lerpv2(p1, p2, t);
		draw_pixel(linea.unpack.x, linea.unpack.y, color);
	}
}

void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color)
{
	Vec2 curva;
	for(float t=0; t<1; t+=0.001)
	{
		curva = lerp2v2(p1, p2, p3, t);
		draw_pixel(curva.unpack.x, curva.unpack.y, color);
	}
}

