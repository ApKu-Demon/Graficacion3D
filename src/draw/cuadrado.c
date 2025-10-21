#include "draw.h"
#include "linea.h"

void draw_cuadrado(int x0, int y0, int largo, int altura, uint32_t color)
{
	int x1 = x0 + largo;
	int y1 = y0;

	int x2 = x1;
	int y2 = y0 + altura;

	int x3 = x0;
	int y3 = y2;

	linea_brhm(x0, y0, x1, y1, color);
	linea_brhm(x1, y1, x2, y2, color);
	linea_brhm(x2, y2, x3, y3, color);
	linea_brhm(x3, y3, x0, y0, color);

}

void fill_cuadro(Vec2 pos, int w, int h, uint32_t color, uint32_t contorno)
{
	for(int y=pos.unpack.y; y<pos.unpack.y+h; y++)
	{
		draw_linea(pos.unpack.x,
					y,
					pos.unpack.x+w,
					y,
					color);
	}

	draw_cuadrado(
		pos.unpack.x, 
		pos.unpack.y,
		w,
		h,
		contorno);
}

