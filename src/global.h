#pragma once

#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "color/colores.h"
#include "draw/figuras.h"
#include "estructuras/mesh.h"

typedef struct _estado
{
	SDL_Renderer *renderer;
	SDL_Texture *textura;
	SDL_Event evento;
	uint32_t *color_buffer;
	int ven_width;
	int ven_height;
	Color clear_color;
	int run;
	Mesh *meshes;
	//Figuras *figuras_buffer;
	//Figuras *figuras_temp_buffer;
}EstadosRender;

extern EstadosRender estadosrender;
extern TTF_Font* font;


