#pragma once

#include "../math/vectores.h"
#include "../draw/figuras.h"
#include "../draw/textura.h"

typedef struct _mesh{
	Vec3 *vertices;
	Vec3 *normales;
	Cara_t *indices;		// indices caras y uvs caras
	Cara_t *n_indices;
	Triangulo *triangulos;	// triangulo a render
	TexturaUV *texturaUV;	// indice del UV
	Textura textura;

	Vec3 rotacion;
	Vec3 escala;
	Vec3 traslado;
}Mesh;

typedef enum{
	VERTICES = 2 << 0,
	NORMALES = 2 << 1,
	INDICES  = 2 << 2,
	UV 		 = 2 << 3,
}MeshOpciones;

Mesh loadMesh(const char* fiilePath, MeshOpciones opt);
