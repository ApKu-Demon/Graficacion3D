#include <math.h>
#include <stdlib.h>
#include "draw.h"
#include "linea.h"

#define PI 3.141592

void draw_circuloTrig(int radio, int x, int y, int numvertices, uint32_t color)
{
	for(int i=0; i<numvertices; i++)
	{
		float angulo = i * (2*PI) / numvertices;
		float angulo_s = (((i + 1) % numvertices) * 2 * PI) / numvertices;
		float xa = x + cos(angulo) * radio;
		float ya = y + sin(angulo) * radio;
		float xs = x + cos(angulo_s) * radio;
		float ys = y + sin(angulo_s) * radio;

		linea_brhm(xa, ya, xs, ys, color);
	}
}


void draw_circuloPM(int radio, int xc, int yc, int borde, uint32_t color)
{
	for (int r = radio - borde + 1; r <= radio; r++)
	{
		int x = 0;
		int y = r;
		int p = 1 - r;

		while(x <= y)
		{
			draw_pixel(xc + x, yc + y, color);
			draw_pixel(xc - x, yc + y, color);
			draw_pixel(xc + x, yc - y, color);
			draw_pixel(xc - x, yc - y, color);
			draw_pixel(xc + y, yc + x, color);
			draw_pixel(xc - y, yc + x, color);
			draw_pixel(xc + y, yc - x, color);
			draw_pixel(xc - y, yc - x, color);
			x++;

			if(p<0)
			{ 
				p += (2 * x) + 1;
			}
			
			else
			{
				y--;
				p += 2 * (x - y) + 1;
			}
		}
	}
}

