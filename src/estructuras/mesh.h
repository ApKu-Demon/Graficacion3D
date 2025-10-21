#pragma once
#include "../math/vectores.h"
#include "../draw/figuras.h"

typedef struct mesh
{
    Vec3 *vertices;
    Vec3 *normales;
    Cara_t *indices;
    //Cara_t *n_indices;
    //Cara_t *t_indices;
    Triangulo *triangulos;

    Vec3 rotacion;
    Vec3 escala;
    Vec3 traslado;
}Mesh;

typedef enum
{
    VERTICES = 2<<0,
    NORMALES = 2<<1,
    INDICES  = 2<<2,
    TEXTURAS = 2<<3,    //UV
}MeshOpciones;

Mesh loadMesh(const char* filePath, MeshOpciones opt);


