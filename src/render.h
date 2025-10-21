#pragma once

/*
Es el archivo interfaz para mostrar en pantalla
*/

#include <stdint.h>
#include "draw/figuras.h"

void update(void);
void copy_buffer_to_texture(void);
void clear_color_buffer(void);
void render_frame(void);
void _init(void);
void render_input(void);

Vec2 *pivote_mas_cerca(Vec2 mp, Figuras* figs, float umbral);

void transformar(void);

int back_face_culling(Vec3 camara, Vec3* puntos);



