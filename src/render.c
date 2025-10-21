#include "render.h"
#include "render.h"
#include "color/colores.h"
#include "draw/draw.h"
#include "draw/linea.h"
#include "global.h"
#include "math/vectores.h"
#include "math/matrix.h"
#include "draw/figuras.h"
#include "memoria/memoria.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Vec3 camara;
Vec3 rotaciones;
Vec3 escalamiento;

const int fovf = 630;

int comparar(const void *a, const void *b)
{
	Triangulo *A = (Triangulo*)a;
	Triangulo *B = (Triangulo*)b;
	return A->avg_z - B->avg_z;
}

void transformar(void)
{
	for(int m=0; m<array_size(estadosrender.meshes); ++m)
	{
		free_array(estadosrender.meshes[m].triangulos);
		estadosrender.meshes[m].triangulos = 0;
		// por cada cara del buffer
		int num_caras = array_size(estadosrender.meshes[m].indices);
		for(int i=0; i<num_caras; i++)
		{
			Cara_t punto_cara = estadosrender.meshes[m].indices[i];	//cubo_caras[i];
			Vec3 cara_vertice[3];
			cara_vertice[0] = estadosrender.meshes[m].vertices[punto_cara.a-1];
			cara_vertice[1] = estadosrender.meshes[m].vertices[punto_cara.b-1];
			cara_vertice[2] = estadosrender.meshes[m].vertices[punto_cara.c-1];

			// por cada vertice
			Triangulo triangulo_proyectado;
			Vec3 vertices_transformados[3];
			for(int j=0; j<3; ++j)
			{
				Vec3 punto = cara_vertice[j];

				// matriz de transformacion
				Mat4 mt = mat4_eye();
				// Matriz identidad
				//print_matriz(&mt);
				
				// escalamos
				mat4_push_escala(&mt, estadosrender.meshes[m].escala);
				
				// rotamos
				mat4_push_rotar(&mt, estadosrender.meshes[m].rotacion);
				
				// trasladamos
				//mat4_push_traslado(&mt, camara);
				mat4_push_traslado(&mt,estadosrender.meshes[m].traslado);

				Vec4 p = {{punto.unpack.x, punto.unpack.y, punto.unpack.z, 1.f}};
			
				p = mat4_dot_vec4(&mt, &p);
				punto = vec4_to_vec3(&p);

				vertices_transformados[j] = punto;
			}
			
			// BACK-FACE CULLING						  
			if(!back_face_culling(camara,vertices_transformados))
				continue;

			float avg_z = (vertices_transformados[0].unpack.z) +
						  (vertices_transformados[1].unpack.z) +
						  (vertices_transformados[2].unpack.z) / 3.f;

			for(int j=0;j<3;++j)
			{
				Vec4 pp = {{vertices_transformados[j].unpack.x,
							vertices_transformados[j].unpack.y,
							vertices_transformados[j].unpack.z, 1.f}};
					
				Mat4 PM = mat4_matriz_proyeccion(fovf, estadosrender.ven_height / (float)estadosrender.ven_width, 1.f, 100.f);
					
				// Matriz proyeccion
				//print_matriz(&PM);

				Vec4 punto_proyectado = proyeccion(&PM,pp);	//proyeccion_perspectiva_div(vertices_transformados[j],fovf);
				//escala ya que los puntos estan entre 0-1 
				punto_proyectado.unpack.y *= estadosrender.ven_height/2.f;
				punto_proyectado.unpack.x *= estadosrender.ven_width/2.f;
				//centrar
				punto_proyectado.unpack.y += estadosrender.ven_height/2.f;
				punto_proyectado.unpack.x += estadosrender.ven_width/2.f;

				//generar triangulo
				triangulo_proyectado.p[j].unpack.x = punto_proyectado.unpack.x;
				triangulo_proyectado.p[j].unpack.y = punto_proyectado.unpack.y;
				triangulo_proyectado.p[j].unpack.z = punto_proyectado.unpack.z;
				triangulo_proyectado.avg_z = avg_z;
			}
			
			//int dummy;
			//scanf("%d", &dummy);
			//cubo_triangulos[i] = triangulo_proyectado;
			pushto_array(estadosrender.meshes[m].triangulos, triangulo_proyectado);
		}

		// painters algorithm oredenar por promedio de profundidad
		qsort(estadosrender.meshes[m].triangulos,
				array_size(estadosrender.meshes[m].triangulos),
				sizeof(estadosrender.meshes[m].triangulos[0]),
				comparar);
	}
}

// No se usa de momento
Vec2 *pivote_mas_cerca(Vec2 mp, Figuras* figs, float umbral)
{	
	int a = 0;
	int b = array_size(figs) - 1;

	float low;
	float high;
	float centro;
	
	for(int i=0; i<array_size(figs); i++)
	{
		int r = (a+b)/2;
		low = distanciav2(mp, figs[a].cuadro.pos);
		high = distanciav2(mp, figs[b].cuadro.pos);
		centro = distanciav2(mp, figs[r].cuadro.pos);

		if(fabs(low) <= umbral)
			return &figs[a].cuadro.pos;
		else if(fabs(high) <= umbral)
			return &figs[b].cuadro.pos;
		else if(fabs(centro) <= umbral)
			return &figs[r].cuadro.pos;

		if(r < b)
			b = r;
		else if(r > a)
			a = r;
	}
	return NULL;
}

void render_input(void)
{
	if(estadosrender.evento.type == SDL_EVENT_QUIT)
	{
		estadosrender.run =0;
	}

	if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
	}

	else if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
	}
}

void clear_color_buffer(void) 
{
	for(int y=0; y<estadosrender.ven_height; ++y) 
	{
		for(int x=0; x<estadosrender.ven_width; ++x) 
		{
			draw_pixel(x, y, estadosrender.clear_color.hex);
		}
	}
}

void copy_buffer_to_texture(void) 
{
	SDL_UpdateTexture(estadosrender.textura,
			NULL,
			estadosrender.color_buffer,
			(int)(estadosrender.ven_width*sizeof(int)));

	SDL_RenderTexture(estadosrender.renderer, 
						estadosrender.textura, 
						NULL, 
						NULL);
}

void _init(void) 
{
	camara.unpack.z = -5.f;

	// cargar mesh
	Mesh cubo = loadMesh("assets/cube.obj", VERTICES | INDICES);

	pushto_array(estadosrender.meshes, cubo);

	estadosrender.meshes[0].rotacion.unpack.x = 0.f;
	estadosrender.meshes[0].rotacion.unpack.y = 0.f;
	estadosrender.meshes[0].rotacion.unpack.z = 0.f;

	estadosrender.meshes[0].escala.unpack.x = 1.f;
	estadosrender.meshes[0].escala.unpack.y = 1.f;
	estadosrender.meshes[0].escala.unpack.z = 1.f;

	estadosrender.meshes[0].traslado.unpack.z = 5.f;

	/*
	// es espacio local, crear el cubo
	int p = 0;
	for(float x=-1; x<=1; x+= 0.25)
	{
		for(float y=-1; y<=1; y+=0.25)
		{
			for(float z=-1; z<=1; z+=0.25)
			{
				Vec3 punto = {{x, y, z}};
				cubo_puntos[p++] = punto;
			}
		}
	}
	*/
}

void update(void) 
{
	estadosrender.meshes[0].rotacion.unpack.x += 0.001f;
	estadosrender.meshes[0].rotacion.unpack.y += 0.001f;
	estadosrender.meshes[0].rotacion.unpack.z += 0.001f;
	transformar();
}

void render_frame(void) 
{
	// por cada mesh
	for(int m=0; m<array_size(estadosrender.meshes); ++m)
	{
		// por cada triangulo
		int num_trian = array_size(estadosrender.meshes[m].triangulos);
		for (int i = 0; i < num_trian; i++) {
			// vertices de los triangulos
			Triangulo trian = estadosrender.meshes[m].triangulos[i];	//cubo_triangulos[i];

			//painter algorithm
			//fill_cuadro(trian.p[0], 4, 4, 0xff00ffff, 0xff00ffff);
			//fill_cuadro(trian.p[1], 4, 4, 0xff00ffff, 0xff00ffff);
			//fill_cuadro(trian.p[2], 4, 4, 0xff00ffff, 0xff00ffff);

			// lineas de los triangulos
			//fill_trian(trian.p[0], trian.p[1], trian.p[2], 0xff00ffff, 0xff00ffff);
			fill_trian(trian);

			draw_trian(trian.p[0].unpack.x, trian.p[0].unpack.y,
						trian.p[1].unpack.x, trian.p[1].unpack.y,
						trian.p[2].unpack.x, trian.p[2].unpack.y,
						0xff00ffff);
    	}
	}

	SDL_RenderPresent(estadosrender.renderer);
}

int back_face_culling(Vec3 camara, Vec3 *puntos)
{
	Vec3 BA = resta_vec3(puntos[1], puntos[0]);
	normalizar_vec3_inplace(&BA);
	Vec3 CA = resta_vec3(puntos[2], puntos[0]);
	normalizar_vec3_inplace(&CA);
	Vec3 N = cross_vec3(BA, CA);
	normalizar_vec3_inplace(&N);
	Vec3 Ray = resta_vec3(camara, puntos[0]);
	normalizar_vec3_inplace(&Ray);
	return dot_vec3(N, Ray) > 0;
}

