#include "render.h"
#include "global.h"
#include "draw/draw.h"
#include "math/matrix.h"
#include "draw/figuras.h"
#include "math/vectores.h"
#include "color/colores.h"
#include "estructuras/luz.h"
#include "memoria/memoria.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_oldnames.h>
#include <math.h>

#define N_PUNTOS 9 * 9 * 9
#define N_CARAS 6 * 2

// Prototipos
void dibujar_gizmo_luz(void);

Triangulo cubo_triangulos[N_CARAS];

Vec3 camara;
Vec3 rotaciones;
Vec3 escalamiento;

/*
int dotsFlag = 0;
int vertexFlag = 0;
int fillFlag = 0;
*/
int render_mesh_mode = 0;

typedef enum _render_mesh_mode
{
	RENDER_MESH_PUNTOS =	2<<0,
	RENDER_MESH_VERTICES =	2<<1,
	RENDER_MESH_RELLENO = 	2<<2,
	RENDER_TEXT_MESH = 		2<<3,
	GIZMO_LUZ =          	2<<4
}RENDER_MESHES_MODES;

int backFaceCullingFlag = 0;

const int fovf = 630;

Luz luz = {{{-2.f, -2.f, 0.f}}};

// ANADIDO
uint32_t *img = 0;

Vec2 *punto_seleccionado = NULL;

// int comparar(const void *a, const void *b){
// 	Triangulo *A = (Triangulo*)a;
// 	Triangulo *B = (Triangulo*)b;

// 	return A -> avg_z - B -> avg_z;
// }

void transformar(void)
{
	for(int m = 0; m < array_size(estadosrender.meshes); ++m)
	{
		free_array(estadosrender.meshes[m].triangulos);
		estadosrender.meshes[m].triangulos = 0;

		// Por cada cara del buffer
		int num_caras = array_size(estadosrender.meshes[m].indices);
		int num_vertices = array_size(estadosrender.meshes[m].vertices);	// para verificar
		for(int i = 0; i < num_caras; i++){
			Cara_t punto_cara = estadosrender.meshes[m].indices[i];
			Vec3 cara_vertice[3];

			// **VERIFICACION CRÍTICA DEL INDICE**
			if (punto_cara.a < 1 || punto_cara.a > num_vertices || 
				punto_cara.b < 1 || punto_cara.b > num_vertices ||
				punto_cara.c < 1 || punto_cara.c > num_vertices) 
			{
				fprintf(stderr, "ERROR CRITICO: Indice de vértice fuera de rango en la cara %d.\n", i);
				fprintf(stderr, "Indices: a=%d, b=%d, c=%d. Max vertices: %d.\n", 
						punto_cara.a, punto_cara.b, punto_cara.c, num_vertices);
				estadosrender.run = 0; // Detiene la aplicación
				continue; // Saltar esta cara corrupta
			}

			cara_vertice[0] = estadosrender.meshes[m].vertices[punto_cara.a - 1];
			cara_vertice[1] = estadosrender.meshes[m].vertices[punto_cara.b - 1];
			cara_vertice[2] = estadosrender.meshes[m].vertices[punto_cara.c - 1];

			// por cada vertice
			Triangulo triangulo_proyectado = {0};
			Vec4 vertices_transformados[3];

			for(int j = 0; j < 3; ++j)
			{
				Vec4 punto = vec3_to_vec4(&cara_vertice[j]);

				// Matriz de transformacion
				Mat4 mt = mat4_eye();
				// Escalamos
				mat4_push_escala(&mt, estadosrender.meshes[m].escala);
				// Rotamos
				mat4_push_rotar(&mt, estadosrender.meshes[m].rotacion);
				// Trasladamos
				mat4_push_traslado(&mt, estadosrender.meshes[m].traslado);

				vertices_transformados[j] = mat4_dot_vec4(&mt, &punto);
			}

			// BACK-FACE CULLING
			//int mostrar = back_face_culling(camara, vertices_transformados);
			//if(!mostrar && (render_mesh_mode & backFaceCullingFlag) > 0)	continue;	// Revisar en affinity_texture #17
			if(!back_face_culling(camara, vertices_transformados) && backFaceCullingFlag)
				continue;

			// avg depth (painters algorithm) TODO: z-buffer depth
			// float avg_z = (vertices_transformados[0].unpack.z + 
			// 	       vertices_transformados[1].unpack.z +
			// 	       vertices_transformados[2].unpack.z) / 3.f;

			triangulo_proyectado.pos[0] = vertices_transformados[0];
			triangulo_proyectado.pos[1] = vertices_transformados[1];
			triangulo_proyectado.pos[2] = vertices_transformados[2];

			// triangulo_proyectado.avg_z = avg_z;
			triangulo_proyectado.color.hex = 0xAB1056FF; //estadosrender.meshes[m].textura.pixeles[0];	//img[0]; //0xAB1056FF;
			normal_triangulo(&triangulo_proyectado);
			float intesidad = -dot_vec3(triangulo_proyectado.normal, luz.direccion);
			triangulo_proyectado.color.hex = luz_intensidad(triangulo_proyectado.color.hex, intesidad);

			triangulo_proyectado.intensidad_luz = intesidad;

			//triangulo_proyectado.normal = normal_triangulo(&triangulo_proyectado);
			Vec4 punto_proyectado[3];
			for(int j = 0; j < 3; ++j)
			{
				//Vec4 pp = {{vertices_transformados[j].unpack.x, vertices_transformados[j].unpack.y, vertices_transformados[j].unpack.z, 1.f}};
				Vec4 pp = vertices_transformados[j];

				Mat4 PM = mat4_matriz_proyeccion(fovf, estadosrender.w_height / (float)estadosrender.w_width, 1.f, 100.f);

				//Vec4 punto_proyectado = proyeccion(&PM, pp);
				punto_proyectado[j] = proyeccion(&PM, pp);

				// Escala ya que los puntos estan entre 0-1
				punto_proyectado[j].unpack.y *= estadosrender.w_height / 2.f;
				punto_proyectado[j].unpack.x *= estadosrender.w_width / 2.f;
				punto_proyectado[j].unpack.y *= -1;

				// Centrar
				punto_proyectado[j].unpack.y += estadosrender.w_height / 2.f;
				punto_proyectado[j].unpack.x += estadosrender.w_width / 2.f;

				// Generar triangulos
				triangulo_proyectado.pos[j].unpack.x = punto_proyectado[j].unpack.x;
				triangulo_proyectado.pos[j].unpack.y = punto_proyectado[j].unpack.y;
				triangulo_proyectado.pos[j].unpack.z = punto_proyectado[j].unpack.z;
				// agregar para la correccion de perspectiva de textura
				triangulo_proyectado.pos[j].unpack.w = punto_proyectado[j].unpack.w;
			}

			// antes de hacer el push a triangulo, agregamos los UV
			// flat shadding
			triangulo_proyectado.texuv[0] = estadosrender.meshes[m].indices[i].a_uv;
			triangulo_proyectado.texuv[1] = estadosrender.meshes[m].indices[i].b_uv;
			triangulo_proyectado.texuv[2] = estadosrender.meshes[m].indices[i].c_uv;

			//estadosrender.meshes[m].triangulos[i] = triangulo_proyectado;
			pushto_array(estadosrender.meshes[m].triangulos, triangulo_proyectado);
		}

		// Painters algorithm ordenar por promedio de profundidad
		// qsort(estadosrender.meshes[m].triangulos, 
		// 	array_size(estadosrender.meshes[m].triangulos), 
		// 	sizeof(estadosrender.meshes[m].triangulos[0]), 
		// 	comparar);
	}
}

void render_input(void) {

    if(estadosrender.evento.type == SDL_EVENT_QUIT) {
        estadosrender.run = 0;
    }

	if (estadosrender.evento.type == SDL_EVENT_KEY_DOWN) {
        switch (estadosrender.evento.key.scancode) {
            case SDL_SCANCODE_1:
                // Alternar RENDER_MESH_PUNTOS (corresponde a dotsFlag)
                render_mesh_mode ^= RENDER_MESH_PUNTOS;
                // printf("Modo: %d\n", render_mode_actual);
                break;

            case SDL_SCANCODE_2:
                // Alternar RENDER_MESH_VERTICES (corresponde a vertexFlag)
                render_mesh_mode ^= RENDER_MESH_VERTICES;
                break;

            case SDL_SCANCODE_3:
                // Alternar RENDER_MESH_RELLENO (corresponde a fillFlag)
                render_mesh_mode ^= RENDER_MESH_RELLENO;
                break;
			
			 case SDL_SCANCODE_4:
                 // backFaceCullingFlag se mantiene como booleano si no lo incluyes en el enum
                 backFaceCullingFlag = !backFaceCullingFlag;
                 break;

			case SDL_SCANCODE_5: 
                // Alternar RENDER_TEXT_MESH
                render_mesh_mode ^= RENDER_TEXT_MESH;
                break;

			case SDL_SCANCODE_6:
				// Dibujamos el gizmo (rayo de luz)
				render_mesh_mode ^= GIZMO_LUZ;
                break;
			
			default:
                break;
        }
    }
}

void clear_color_buffer(){
    for(int y = 0; y < estadosrender.w_height; ++y){
        for(int x = 0; x < estadosrender.w_width; ++x){
            draw_pixel(x, y, estadosrender.clear_color.hex);
        }
    }
}

void clear_z_buffer(void)
{
	for(int y=0; y<estadosrender.w_height; ++y)
	{
		for(int x=0; x<estadosrender.w_width; ++x)
		{
			estadosrender.z_buffer[y * estadosrender.w_width + x] = 1.f;
		}
	}
}

void copy_buffer_to_texture(){
    SDL_UpdateTexture(estadosrender.textura, 
                      NULL, 
                      estadosrender.color_buffer, 
                      (int)(estadosrender.w_width * sizeof(int)));

    SDL_RenderTexture(estadosrender.renderer, 
                      estadosrender.textura, 
                      NULL, 
                      NULL);
}

void _Init(){
	camara.unpack.z = -5.f;

	// Cargar meshes
	Mesh cubo = loadMesh("assets/crab.obj", VERTICES | INDICES | UV);
	pushto_array(estadosrender.meshes, cubo);

	Mesh suelo = loadMesh("assets/suelo3D.obj", VERTICES | INDICES | UV);
	pushto_array(estadosrender.meshes, suelo);

	estadosrender.meshes[0].rotacion.unpack.x = 0.f;
	estadosrender.meshes[0].rotacion.unpack.y = 0.f;
	estadosrender.meshes[0].rotacion.unpack.z = 0.f;

	estadosrender.meshes[0].escala.unpack.x = 1.f;
	estadosrender.meshes[0].escala.unpack.y = 1.f;
	estadosrender.meshes[0].escala.unpack.z = 1.f; 
	
	estadosrender.meshes[0].traslado.unpack.z = 5.f;

	int imgx, imgy, imgcomp;
	estadosrender.meshes[0].textura.pixeles = cargar_imagen("assets/crab.png", &imgx, &imgy, &imgcomp, 4);
	estadosrender.meshes[0].textura.width = imgx;
	estadosrender.meshes[0].textura.height = imgy;
	printf("(%d, %d, %d)\n", imgx, imgy, imgcomp);

	estadosrender.meshes[1].rotacion.unpack.x = 0.f;
	estadosrender.meshes[1].rotacion.unpack.y = 0.f;
	estadosrender.meshes[1].rotacion.unpack.z = 0.f;

	estadosrender.meshes[1].escala.unpack.x = 2.f;
	estadosrender.meshes[1].escala.unpack.y = 2.f;
	estadosrender.meshes[1].escala.unpack.z = 1.f; 
	
	estadosrender.meshes[1].traslado.unpack.y = -10.f;
	estadosrender.meshes[1].traslado.unpack.z = 30.f;

	int imgx1, imgy1, imgcomp1;
	estadosrender.meshes[1].textura.pixeles = cargar_imagen("assets/playa.jpg", &imgx1, &imgy1, &imgcomp1, 4);
	estadosrender.meshes[1].textura.width = imgx1;
	estadosrender.meshes[1].textura.height = imgy1;
	printf("(%d, %d, %d)\n", imgx1, imgy1, imgcomp1);

	/*
	img = cargar_imagen("assets/test.png", &imgx, &imgy, &imgcomp, 3);
	if (!img) {
        fprintf(stderr, "Error: No se pudo cargar la imagen de textura.\n");
        // Manejar el fallo, posiblemente salir del programa o usar un color por defecto.
    }
	printf("(%d, %d, %d)\n", imgx, imgy, imgcomp);
	*/
}

void update(){
	// estadosrender.meshes[0].rotacion.unpack.x += 0.002f;
	estadosrender.meshes[0].rotacion.unpack.y += 0.002f;
	// estadosrender.meshes[0].rotacion.unpack.z += 0.002f;

	// estadosrender.meshes[1].rotacion.unpack.x += 0.002f;
	estadosrender.meshes[1].rotacion.unpack.y += 0.002f;
	// estadosrender.meshes[1].rotacion.unpack.z += 0.002f;

	transformar();
}

void render_frame(){
	// Por cada mesh
	for(int m = 0; m < array_size(estadosrender.meshes); ++m)
	{
		// Por cada triangulo
		int num_trian = array_size(estadosrender.meshes[m].triangulos);
		for(int i = 0; i < num_trian; i++)
		{
			// Vertices de los triangulos
			Triangulo trian = estadosrender.meshes[m].triangulos[i];

			// relleno de los triangulos
			if((render_mesh_mode & RENDER_MESH_RELLENO) > 0){
				//fill_triangulo(&trian, trian.color.hex);
				fill_trian( trian.pos[0],
							trian.pos[1],
							trian.pos[2], trian.color.hex);
			}

			// textura
			if((render_mesh_mode & RENDER_TEXT_MESH) > 0)
			{
				tex_trian(	trian.pos[0], trian.texuv[0],
							trian.pos[1], trian.texuv[1],
							trian.pos[2], trian.texuv[2],
							estadosrender.meshes[m].textura.pixeles, 
							estadosrender.meshes[m].textura.width, 
							estadosrender.meshes[m].textura.height,
							trian.intensidad_luz);
			}

			// aristas
			if((render_mesh_mode & RENDER_MESH_VERTICES) > 0){
				draw_trian( trian.pos[0].unpack.x, trian.pos[0].unpack.y,
					   		trian.pos[1].unpack.x, trian.pos[1].unpack.y,
				   	   		trian.pos[2].unpack.x, trian.pos[2].unpack.y, 0x00FFFFFF);
			}

			// vertices
			if((render_mesh_mode & RENDER_MESH_PUNTOS) > 0){
				fill_cuadro(vec4_to_vec3(&trian.pos[0]), 4, 4, 0x0000FFFF, 0x000000FF);
				fill_cuadro(vec4_to_vec3(&trian.pos[1]), 4, 4, 0x0000FFFF, 0x000000FF);
				fill_cuadro(vec4_to_vec3(&trian.pos[2]), 4, 4, 0x0000FFFF, 0x000000FF);
			}
		}
	}

	// Gizmo luz
	if((render_mesh_mode & GIZMO_LUZ) > 0)
		dibujar_gizmo_luz();
	
	SDL_RenderPresent(estadosrender.renderer);
}

int back_face_culling(Vec3 camara, Vec4 *puntos){
	Vec3 BA = resta_vec3(vec4_to_vec3(&puntos[1]), vec4_to_vec3(&puntos[0]));
	normalizar_vec3_inplace(&BA);
	Vec3 CA = resta_vec3(vec4_to_vec3(&puntos[2]), vec4_to_vec3(&puntos[0]));
	normalizar_vec3_inplace(&CA);

	Vec3 N = cross_vec3(BA, CA);
	normalizar_vec3_inplace(&N);
	Vec3 Ray = resta_vec3(camara, vec4_to_vec3(&puntos[0]));
	normalizar_vec3_inplace(&Ray);

	return dot_vec3(N, Ray) > 0;
}

void dibujar_gizmo_luz(void) {
    if (array_size(estadosrender.meshes) == 0) return;

    // 1. OBJETIVO (Target): El centro de tu objeto (Mesh 0)
    Vec3 target_pos = estadosrender.meshes[0].traslado;

    // 2. ORIGEN (Source): Calculamos de donde viene la luz
    // Invertimos la direccion y nos alejamos 4 unidades del objeto
    Vec3 dir_luz = luz.direccion;
    normalizar_vec3_inplace(&dir_luz);
    
    // source = target - (direccion * distancia)
    Vec3 source_pos = resta_vec3(target_pos, escala_vec3(&dir_luz, 4.0f));

    Vec3 puntos_3d[2] = {source_pos, target_pos};
    Vec2 puntos_screen[2];

    for(int i = 0; i < 2; i++) {
        Vec4 p_mundo = vec3_to_vec4(&puntos_3d[i]);
        
        // Usamos la misma matriz de proyeccion del render principal
        Mat4 PM = mat4_matriz_proyeccion(fovf, estadosrender.w_height / (float)estadosrender.w_width, 1.f, 100.f);
        Vec4 p_proj = proyeccion(&PM, p_mundo);

        // Transformar a coordenadas de pantalla
        p_proj.unpack.x *= estadosrender.w_width / 2.f;
        p_proj.unpack.y *= estadosrender.w_height / 2.f;
        p_proj.unpack.y *= -1; // Invertir Y

        p_proj.unpack.x += estadosrender.w_width / 2.f;
        p_proj.unpack.y += estadosrender.w_height / 2.f;

        puntos_screen[i].unpack.x = p_proj.unpack.x;
        puntos_screen[i].unpack.y = p_proj.unpack.y;
    }

    // Linea Amarilla (Rayo de luz)
    draw_linea(
        (int)puntos_screen[0].unpack.x, (int)puntos_screen[0].unpack.y,
        (int)puntos_screen[1].unpack.x, (int)puntos_screen[1].unpack.y,
        0xFFFF00FF 
    );

    // Circulo Rojo (Origen de la luz - "La Luz")
    draw_circuloPM(9, (int)puntos_screen[0].unpack.x, (int)puntos_screen[0].unpack.y, 0xFF0000FF);

    // Circulo Verde (Destino - "El Objeto")
    draw_circuloPM(3, (int)puntos_screen[1].unpack.x, (int)puntos_screen[1].unpack.y, 0x00FF00FF);

}


