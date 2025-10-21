#include "figuras.h"
#include "draw.h"
#include "../estructuras/pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// VARIABLES GLOBALES
#define VEN_WIDTH 1080
#define VEN_HEIGHT 720

//PROTOTIPOS
void fill(Vec2 pos, uint32_t color);
//bool dentro_fig(int x, int y, Figuras* fig);



void draw_figura(Figuras *fig)
{
	switch(fig->data.type)
	{
		case LINEA:
		{
			Linea f = fig->linea;
			draw_circuloPM(5.f, f.p1.unpack.x, f.p1.unpack.y, 2, 0x0000ffff);
			draw_circuloTrig(6.f, f.p2.unpack.x, f.p2.unpack.y, 32, 0x0000ffff);
			draw_linea(f.p1.unpack.x, f.p1.unpack.y, f.p2.unpack.x, f.p2.unpack.y, f.color.hex);
		}
		break;

		case CURVA:
		{
			Curva c = fig->curva;
			draw_circuloPM(5.f, c.p1.unpack.x, c.p1.unpack.y, 1, 0x0000ffff);
			draw_circuloPM(5.f, c.p2.unpack.x, c.p2.unpack.y, 1, 0x0000ffff);
			draw_circuloPM(5.f, c.p3.unpack.x, c.p3.unpack.y, 1, 0x0000ffff);
			draw_curva(c.p1, c.p2, c.p3, c.color.hex);
		}
		break;

		case CIRC:
		{
			Circulo circ = fig->circulo;
			draw_circuloTrig(circ.r, circ.pos.unpack.x, circ.pos.unpack.y, circ.vert, circ.color.hex);
		}
		break;
		
		case CUADRO:
		{
			Cuadro cuad = fig->cuadro;
			draw_circuloPM(5.f, cuad.pos.unpack.x, cuad.pos.unpack.y, 1, 0x0000ffff);
			draw_circuloPM(5.f, (cuad.pos.unpack.x + cuad.w), cuad.pos.unpack.y, 1, 0x0000ffff);
			draw_circuloPM(5.f, (cuad.pos.unpack.x + cuad.w), (cuad.pos.unpack.y + cuad.h), 1, 0x0000ffff);
			draw_circuloPM(5.f, cuad.pos.unpack.x, (cuad.pos.unpack.y + cuad.h), 1, 0x0000ffff);
			draw_cuadrado(cuad.pos.unpack.x, cuad.pos.unpack.y, cuad.w, cuad.h, cuad.color.hex);
		}
		break;


		case TRIAN:
		{
			Triangulo triangulo = fig->triangulo;
			draw_circuloPM(5.f, triangulo.p[0].unpack.x, triangulo.p[0].unpack.y, 1, 0x0000ffff);
			draw_circuloPM(5.f, triangulo.p[1].unpack.x, triangulo.p[1].unpack.y, 1, 0x0000ffff);
			draw_circuloPM(5.f, triangulo.p[2].unpack.x, triangulo.p[2].unpack.y, 1, 0x0000ffff);
			draw_trian(	triangulo.p[0].unpack.x, triangulo.p[0].unpack.y,
					triangulo.p[1].unpack.x, triangulo.p[1].unpack.y,
					triangulo.p[2].unpack.x, triangulo.p[2].unpack.y,
					triangulo.color.hex);
		}
		break;
		
		default:
		{}break;

	}
}

/*
void fill_figura(Figuras *fig)
{
	switch(fig->data.type)
	{
		case TRIAN:
		{
			Triangulo triangulo = fig->triangulo;
			fill_trian(triangulo);
		}
		break;
		
		default:
		{}break;

	}
}
*/

void fill_figura(Figuras *fig)
{
	switch(fig->data.type)
	{
		case TRIAN:
		{
			Triangulo triangulo = fig->triangulo;
			fill_trian(triangulo);
		}
		break;
		
		
		case CIRC:
		{
			Circulo circ = fig->circulo;
			fill(circ.pos, circ.color.hex);
		}
		break;
		
		
		case CUADRO:
		{
			Cuadro cuad = fig->cuadro;
			Vec2 pos_inicio = cuad.pos;
			pos_inicio.unpack.x += 1;
			pos_inicio.unpack.y += 1;

			fill(pos_inicio, cuad.color.hex);
		}
		break;

		default:
		{}break;
	}

}



void fill(Vec2 pos, uint32_t color) 
{
	Pila pila = {NULL, 0};
	Vec2* pixel = malloc(sizeof(Vec2));

	bool mapa_visitados[VEN_WIDTH][VEN_HEIGHT] = {false};

	*pixel = pos;
	push(&pila, pixel);
	mapa_visitados[(int)pixel->unpack.x][(int)pixel->unpack.y] = true;

	do
	{
		Vec2* actual = (Vec2*)pop(&pila);
		if (!actual) continue;

		int x = (int)actual->unpack.x;
        int y = (int)actual->unpack.y;
		//uint32_t color_actual = get_colorPixel(x,y);

		//if(!dentro_fig(x, y, fig)) continue;

		/*
		if(color_actual == color)
		{
			free(actual);
			continue;
		}
		*/

		draw_pixel(x, y, color);

		Vec2 vecinos[4] = {
			{{x, y - 1}},		// N
			{{x - 1, y}},		// O
			{{x, y + 1}},		// S
			{{x + 1, y}},		// E
		};

		for (int i = 0; i < 4; i++)
		{
			int nx = (int)vecinos[i].unpack.x;
            int ny = (int)vecinos[i].unpack.y;

			if (nx >= 0 && ny >= 0 && nx < VEN_WIDTH && ny < VEN_HEIGHT)
			{
				uint32_t vecino_color = get_colorPixel(nx, ny);

				if(!mapa_visitados[nx][ny] && vecino_color != color)
				{
					Vec2* nuevo = malloc(sizeof(Vec2));
                    *nuevo = vecinos[i];

					push(&pila, nuevo);
                    mapa_visitados[nx][ny] = true;
				}
			}
		}

		free(actual);

	}while(pila.cantidad);
}



/*
bool dentro_fig(int x, int y, Figuras* fig) {
    switch(fig->data.type) {
        case CIRC:
		{
            int dx = x - (int)fig->circulo.pos.unpack.x;
            int dy = y - (int)fig->circulo.pos.unpack.y;
            return dx*dx + dy*dy <= fig->circulo.r * fig->circulo.r;
		}
        break;

        case CUADRO:
            return x >= (int)fig->cuadro.pos.unpack.x &&
                   x <  (int)fig->cuadro.pos.unpack.x + fig->cuadro.w &&
                   y >= (int)fig->cuadro.pos.unpack.y &&
                   y <  (int)fig->cuadro.pos.unpack.y + fig->cuadro.h;
		break;

		default:
		{
			return false;
		}break;
    }
}
*/

