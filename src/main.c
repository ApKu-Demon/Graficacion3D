#include <SDL3/SDL.h>
//#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "global.h"
#include "render.h"
#include "memoria/memoria.h"

// Window Flags
//#define SDL_WINDOW_OPENGL      SDL_UINT64_C(0X0000000000000002)

// VARIABLES GLOBALES # # # # # # # # # # # #
#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)

int frame_tiempo_prev = 0;
int grabar = 0;

SDL_Window *ventana = 0;



// PROTOTIPOS # # # # # # # # # # # #

void render_init(void);
void render_clean(void);
void grabar_imagen(void);



// Para Windows: main(int argc, char *argv[])

int main(void) 
{
	printf("Hola mundo!!\n");

	render_init();
	_init();
	while(estadosrender.run)
	{
		int delay_frame = FRAME_TARGET_TIME - (SDL_GetTicks() - frame_tiempo_prev);

		if(delay_frame > 0 && delay_frame <= FRAME_TARGET_TIME){
			SDL_Delay(delay_frame);
		}
		
		// TODO: cambiar a funcion
		while(SDL_PollEvent(&estadosrender.evento))
		{
			render_input();
			if (estadosrender.evento.type == SDL_EVENT_KEY_DOWN)
			{
				// Si tecla 1 se presiona (tecla uno para descargar)
				if(estadosrender.evento.key.scancode == SDL_SCANCODE_1)
				{
					grabar = 1;
				}
			}
		}
		
		update();

		if(grabar)
		{
			grabar_imagen();
			printf("Se grabo la imagen\n");
			grabar = 0;
		}

		copy_buffer_to_texture();
		clear_color_buffer();
		render_frame();
	}

	render_clean();

	return 0;

}


// FUNCIONES # # # # # # # # # # # #

void render_init(void)
{
	SDL_CreateWindowAndRenderer("Motor CPU",
		estadosrender.ven_width, 
		estadosrender.ven_height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, //SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		&ventana, 
		&estadosrender.renderer);

	assert(ventana && "no se pudo crear la ventana\n");
	assert(estadosrender.renderer && "no se pudo crear el renderer\n");
        

	estadosrender.color_buffer = malloc(
		estadosrender.ven_width*estadosrender.ven_height*sizeof(uint32_t));

	assert(estadosrender.color_buffer && "No se pudo crear el color buffer\n");

	estadosrender.textura = SDL_CreateTexture(
		estadosrender.renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		estadosrender.ven_width,
		estadosrender.ven_height);

	assert(estadosrender.textura && "no se pudo crear la textura\n");
}

void render_clean(void)
{
	free_array(estadosrender.meshes);
	//free_array(estadosrender.figuras_buffer);
	//free_array(estadosrender.figuras_temp_buffer);
	SDL_DestroyTexture(estadosrender.textura);
	SDL_DestroyRenderer(estadosrender.renderer);
	SDL_DestroyWindow(ventana);
	free(estadosrender.color_buffer);
}

void grabar_imagen(void){
    FILE *file = fopen("imagen.ppm", "w");

    fprintf(file, "%s\n", "P3");
    fprintf(file, "%d %d\n", estadosrender.ven_width, estadosrender.ven_height);
    fprintf(file, "%d\n", 255);

    for(int i = 0; i < estadosrender.ven_height * estadosrender.ven_width; ++i){
        Color pixel = {estadosrender.color_buffer[i]};
        fprintf(file, "%d %d %d\n", pixel.rgba.r, pixel.rgba.g, pixel.rgba.b);
    }

    fclose(file);
}


