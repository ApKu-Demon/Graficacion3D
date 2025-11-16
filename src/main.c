#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "global.h"
#include "render.h"
#include "draw/linea.h"
#include "memoria/memoria.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)
int frame_tiempo_prev = 0;
int grabar = 0;

void render_init(void);
void render_clean(void);
void grabar_imagen(void);

//------------------ Window Flags ------------------//
#define SDL_WINDOW_OPENGL   SDL_UINT64_C(0x0000000000000002)    /**< window usable with OpenGL context */

//----------------- Visual Pointers ---------------//
SDL_Window *window = NULL;

int main(int argc, char** argv){
    
    render_init();
    
    _Init();
    

    while(estadosrender.run){
        int delay_frame = FRAME_TARGET_TIME -(SDL_GetTicks()-frame_tiempo_prev);
        
        if(delay_frame > 0 && delay_frame <= FRAME_TARGET_TIME){
            SDL_Delay(delay_frame);
        }

        while(SDL_PollEvent(&estadosrender.evento)){            
            render_input();
            if(estadosrender.evento.type == SDL_EVENT_KEY_DOWN){
                // Si tecla 1 se presiona
                if(estadosrender.evento.key.scancode == SDL_SCANCODE_CAPSLOCK){
                    grabar = 0;
                }
            }
	}
        
            update();
            if(grabar){
                grabar_imagen();
                printf("Se grabo la imagen\n");
                grabar = 0;
            }
            copy_buffer_to_texture();
            clear_color_buffer();
            render_frame();

	    frame_tiempo_prev = delay_frame;
        }
    render_clean();

    return 0;
}

void render_init(void){
    SDL_CreateWindowAndRenderer("Window test", 
                                estadosrender.w_width, 
                                estadosrender.w_height, 
                                SDL_WINDOW_OPENGL, 
                                &window, 
                                &estadosrender.renderer);

    assert(window && "No se creó la ventana...\n");
    assert(estadosrender.renderer && "No se creó el render...\n");

    estadosrender.color_buffer = malloc(estadosrender.w_width * estadosrender.w_height * sizeof(uint32_t));
    assert(estadosrender.color_buffer && "No se creó el color buffer...\n");

    estadosrender.texture = SDL_CreateTexture(estadosrender.renderer, 
                                              SDL_PIXELFORMAT_RGBA8888, 
                                              SDL_TEXTUREACCESS_STREAMING, 
                                              estadosrender.w_width, 
                                              estadosrender.w_height);
    assert(estadosrender.texture && "No se creó la textura...\n");
}

void render_clean(void){
    free_array(estadosrender.meshes);
    //free(estadosrender.figuras_buffer);
    SDL_DestroyTexture(estadosrender.texture);
    SDL_DestroyRenderer(estadosrender.renderer);
    SDL_DestroyWindow(window);
    free(estadosrender.color_buffer);
}

void grabar_imagen(void){
    FILE *file = fopen("imagen.ppm", "w");

    fprintf(file, "%s\n", "P3");
    fprintf(file, "%d %d\n", estadosrender.w_width, estadosrender.w_height);
    fprintf(file, "%d\n", 255);

    for(int i = 0; i < estadosrender.w_height * estadosrender.w_width; ++i){
        Color pixel = {estadosrender.color_buffer[i]};
        fprintf(file, "%d %d %d\n", pixel.rgba.r, pixel.rgba.g, pixel.rgba.b);
    }

    fclose(file);
}
