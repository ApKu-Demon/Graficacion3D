#include "draw.h"
#include "linea.h"
#include "../math/vectores.h"
#include "figuras.h"
#include <stdio.h>

// Prototipos
void acomodo_vec(Triangulo *triangulo);
void fill_flat_bottom(Vec3 p1, Vec3 p3, Vec3 c, uint32_t color);
void fill_flat_top(Vec3 p1, Vec3 c, Vec3 p2, uint32_t color);

void draw_trian(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	linea_brhm(x0, y0, x1, y1, color);
	linea_brhm(x1, y1, x2, y2, color);
	linea_brhm(x2, y2, x0, y0, color);
}

void fill_trian(Triangulo triangulo)
{
	Triangulo *trian = &triangulo;
	acomodo_vec(trian);

	if(triangulo.p[1].unpack.y == triangulo.p[2].unpack.y)
	{
		printf("entre");
		fill_flat_bottom(triangulo.p[0], triangulo.p[2], triangulo.p[1], triangulo.color.hex);
	}
	else if(triangulo.p[0].unpack.y == triangulo.p[1].unpack.y)
	{
		fill_flat_top(triangulo.p[0], triangulo.p[2], triangulo.p[1], triangulo.color.hex);
	}
	else{
		int cy = triangulo.p[1].unpack.y;
		int cx = (triangulo.p[1].unpack.y - triangulo.p[0].unpack.y) * (triangulo.p[2].unpack.x - triangulo.p[0].unpack.x) / (triangulo.p[2].unpack.y - triangulo.p[0].unpack.y) + triangulo.p[0].unpack.x;

		Vec3 aux;
		aux.unpack.x = cx;
		aux.unpack.y = cy; 

		fill_flat_bottom(triangulo.p[0], aux, triangulo.p[1], triangulo.color.hex);
		fill_flat_top(aux, triangulo.p[1], triangulo.p[2], triangulo.color.hex);
	}
}

// . . . . . . . . . .


void fill_flat_bottom(Vec3 p1, Vec3 p3, Vec3 c, uint32_t color)
{
	// Clacula las pendientes
    float mi = (p3.unpack.x - p1.unpack.x) / (p3.unpack.y - p1.unpack.y); // pendiente izquierda
    float mf = (c.unpack.x - p1.unpack.x) / (c.unpack.y - p1.unpack.y);   // pendiente derecha

    // Inicializa las posiciones x para cada lado
    float xi = p1.unpack.x; // x izquierda
    float xf = p1.unpack.x; // x derecha

    // // linea de p1.y hasta c.y (de arriba hacia abajo)
    for (int y = (int)p1.unpack.y; y <= (int)c.unpack.y; y++) {
        linea_brhm(xi, y, xf, y, color); // dibuja una línea horizontal entre xi y xf
        xi += mi; // avanza x izquierda según la pendiente
        xf += mf; // avanza x derecha según la pendientes
    }
}

void fill_flat_top(Vec3 p1, Vec3 c, Vec3 p2, uint32_t color) {
    // Calcula las pendientes
    float mi = (c.unpack.x - p1.unpack.x) / (c.unpack.y - p1.unpack.y); // pendiente izquierda
    float mf = (c.unpack.x - p2.unpack.x) / (c.unpack.y - p2.unpack.y);   // pendiente derecha

    // Inicializa las posiciones x para cada lado
    float xi = c.unpack.x;
    float xf = c.unpack.x;

    // linea de p2.y hasta p1.y (de arriba hacia abajo)
    for (int y = (int)c.unpack.y; y >= (int)p1.unpack.y; y--) {
        draw_linea(xi, y, xf, y, color); // dibuja una linea horizontal entre xi y xf
        xi -= mi; // retrocede x izquierda segun la pendiente
        xf -= mf; // retrocede x derecha segun la pendiente
    }
}

void acomodo_vec(Triangulo *triangulo)
{
	//Vec3 *vectores[3] = {&triangulo->p[0], &triangulo->p[1], &triangulo->p[2]};
	Vec3 aux;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(triangulo->p[i].unpack.y > triangulo->p[j].unpack.y)
			{
				aux = triangulo->p[i];
				triangulo->p[i] = triangulo->p[j];
				triangulo->p[j] = aux;
			}
		}
	}
}

Vec3 normal_triangulo(Triangulo *trian)
{
	Vec3 AB = resta_vec3(trian->p[1], trian->p[0]);
	Vec3 AC = resta_vec3(trian->p[2], trian->p[0]);
	normalizar_vec3_inplace(&AB);
	normalizar_vec3_inplace(&AC);

	Vec3 normal = cross_vec3(AC, AB);
	normalizar_vec3_inplace(&normal);

	return normal;
}


