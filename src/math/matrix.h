#pragma once

#include "vectores.h"

#include <math.h>

typedef struct _mat4{
    float data[4 * 4];
}Mat4;

void print_matriz(Mat4 *m);
Mat4 mat4_eye(void);

void mat4_push_escala(Mat4 *matriz, Vec3 escala);
void mat4_push_traslado(Mat4 *matriz, Vec3 pos);
void mat4_push_rotar_x(Mat4 *matriz, float radianes);
void mat4_push_rotar_y(Mat4 *matriz, float radianes);
void mat4_push_rotar_z(Mat4 *matriz, float radianes);
void mat4_push_rotar(Mat4 *matriz, Vec3 rotacion);

Vec4 mat4_dot_vec4(Mat4 *matriz, Vec4 *punto);

// Proyeccion
void mat4_push_proyeccion_ortogonal(Mat4 *matriz, float fovf);
void mat4_push_proyeccion_perspectiva(Mat4 *matriz, float fovf);

Vec2 proyeccion_ortografica(Vec3 punto, float fovf);
Vec2 proyeccion_perspectiva(Vec3 punto, float fovf);
Vec4 proyeccion(Mat4 *mat, Vec4 p);

Mat4 mat4_matriz_proyeccion(float fov, float aspect, float znear, float zfar);

