#include "mesh.h"
#include "../memoria/memoria.h"

#include <stdio.h>

Mesh loadMesh(const char *filePath, MeshOpciones opt){
	Mesh nuevo = {0};
	FILE *mesh_file = fopen(filePath, "r");
	char renglon[256];

	while(fgets(renglon, 256, mesh_file)){
        if(renglon[0] == '\0' || renglon[0] == '#' || renglon[0] == '\n')
            continue;
        
        // 1. Lectura de Coordenadas de Textura 'vt' (PRIORIDAD ALTA)
        // Movemos este bloque al principio para que nuevo.texturaUV se llene
        // antes de que cualquier cara 'f' intente referenciarlo.
        if((renglon[0] == 'v' && renglon[1] == 't') && (opt & UV) == UV)
        {
            TexturaUV uv = {0};
            sscanf(renglon + 2, "%f %f", &uv.u, &uv.v); 
            pushto_array(nuevo.texturaUV, uv);
        }
        
        // 2. Lectura de Vértices 'v'
        // Lo dejamos en segundo lugar ya que no accede a otros arrays.
        else if ((renglon[0] == 'v' && renglon[1] == ' ') & ((opt & VERTICES) == VERTICES)) {
            Vec3 vertice = {{0}};
            sscanf(renglon + 2, "%f %f %f", &vertice.unpack.x, &vertice.unpack.y, &vertice.unpack.z);
            pushto_array(nuevo.vertices, vertice);
        }
        
        // 3. Lectura de Caras o Indices 'f' (PRIORIDAD BAJA)
        // Lo dejamos al final porque este bloque REFERENCIA los arrays llenados arriba.
        else if ((renglon[0] == 'f' && renglon[1] == ' ') && ((opt & INDICES) == INDICES)) {
            printf("Procesando caras\n");
            Cara_t vertice_id = {0};
            Cara_t normal_id = {0};
            Cara_t textura_id = {0};
            
            sscanf(renglon + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertice_id.a, &textura_id.a, &normal_id.a,
                    &vertice_id.b, &textura_id.b, &normal_id.b,
                    &vertice_id.c, &textura_id.c, &normal_id.c);

            // Proteger el acceso al array UV
            int num_uvs_cargados = array_size(nuevo.texturaUV); 
            
            // Verificamos si la opcion UV esta activa Y si el indice de textura 
            // mas grande es menor o igual al número de UVs cargados.
            if ((opt & UV) == UV && num_uvs_cargados >= textura_id.a && 
                num_uvs_cargados >= textura_id.b && num_uvs_cargados >= textura_id.c) 
            {
                // Acceso seguro: el array ya está lleno
                vertice_id.a_uv = nuevo.texturaUV[textura_id.a - 1];
                vertice_id.b_uv = nuevo.texturaUV[textura_id.b - 1];
                vertice_id.c_uv = nuevo.texturaUV[textura_id.c - 1];
            } else {
                 // Si falla la verificacion usamos valores por defecto (0, 0) y no accedemos a memoria invalida.
                 vertice_id.a_uv = (TexturaUV){0};
                 vertice_id.b_uv = (TexturaUV){0};
                 vertice_id.c_uv = (TexturaUV){0};
                 
                 if ((opt & UV) == UV) {
                    fprintf(stderr, "ADVERTENCIA: Fallo al referenciar UVs en una cara. Datos de UV perdidos.\n");
                 }
            }

            pushto_array(nuevo.indices, vertice_id);
            pushto_array(nuevo.n_indices, normal_id);
        }
    }

	// OPCIONAL
	if (mesh_file) {
        fclose(mesh_file);
    }

	/*
	while(fgets(renglon, 256, mesh_file)){
		if(renglon[0] == '\0' || renglon[0] == '#' || renglon[0] == '\n')
			continue;

		// Vertices
		if ((renglon[0] == 'v' && renglon[1] == ' ') & ((opt & VERTICES) == VERTICES)) {
			printf("Procesando vertice\n");

			Vec3 vertice = {{0}};
			sscanf(renglon + 2, "%f %f %f", &vertice.unpack.x, &vertice.unpack.y, &vertice.unpack.z);

			pushto_array(nuevo.vertices, vertice);
		// Caras o indices
		} else if ((renglon[0] == 'f' && renglon[1] == ' ') && ((opt & INDICES) == INDICES)) {
			printf("Procesando caras\n");
			Cara_t vertice_id;
			Cara_t normal_id;
			Cara_t textura_id;

			sscanf(renglon + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
					&vertice_id.a, &textura_id.a, &normal_id.a,
					&vertice_id.b, &textura_id.b, &normal_id.b,
					&vertice_id.c, &textura_id.c, &normal_id.c);
			
			int num_uvs_cargados = array_size(nuevo.texturaUV);

			if (nuevo.texturaUV == NULL) {
                 fprintf(stderr, "ADVERTENCIA: Caras 'f' antes de coordenadas 'vt' o UV no seleccionado.\n");
                 // Aquí deberías saltar el procesamiento de UV si no es posible.
            }

			//uv, se supone que primero guardamos los indices en un arreglo
			vertice_id.a_uv = nuevo.texturaUV[textura_id.a - 1];
			vertice_id.b_uv = nuevo.texturaUV[textura_id.b - 1];
			vertice_id.c_uv = nuevo.texturaUV[textura_id.c - 1];

			pushto_array(nuevo.indices, vertice_id);
			pushto_array(nuevo.n_indices, normal_id);
		}
		else if((renglon[0] == 'v' && renglon[1] == 't') && (opt & UV) == UV)
		{
			// Cargamos primeros los indices de los uv en un arreglo
			// para despues referenciar en los uv de cada vertice.
			TexturaUV uv = {0};
			sscanf(renglon+2, "%f %f", &uv.u, &uv.v);
			pushto_array(nuevo.texturaUV, uv);
		}
	}
	*/

	nuevo.escala = (Vec3){{1.f, 1.f, 1.f}};
	nuevo.rotacion = (Vec3){{0.f, 0.f, 0.f}};
	nuevo.traslado = (Vec3){{0.f, 0.f, 0.f}};

	return nuevo;
}
