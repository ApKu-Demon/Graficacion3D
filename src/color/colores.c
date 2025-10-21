#include "colores.h"

Color lerp_color(Color a, Color b, float t)
{
	Color nuevo;
	nuevo.rgba.r = a.rgba.r + (b.rgba.r - a.rgba.r) * t;
	nuevo.rgba.g = a.rgba.g + (b.rgba.g - a.rgba.g) * t;
	nuevo.rgba.b = a.rgba.b + (b.rgba.b - a.rgba.b) * t;

	nuevo.rgba.a = 0xff;
	return nuevo;
}


