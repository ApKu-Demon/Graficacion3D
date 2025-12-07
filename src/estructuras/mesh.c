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

        // vertices
        if ((renglon[0] == 'v' && renglon[1] == ' ') & ((opt & VERTICES) == VERTICES)) {
            //printf("Procesando vertice\n");
            Vec3 vertice = {{0}};
            sscanf(renglon + 2, "%f %f %f", &vertice.unpack.x, &vertice.unpack.y, &vertice.unpack.z);
            pushto_array(nuevo.vertices, vertice);
        }

        // caras o indices
        else if ((renglon[0] == 'f' && renglon[1] == ' ') && ((opt & INDICES) == INDICES)) {
            //printf("Procesando caras\n");
            Cara_t vertice_id;
            Cara_t normal_id;
            Cara_t textura_id;
            
            sscanf(renglon + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertice_id.a, &textura_id.a, &normal_id.a,
                    &vertice_id.b, &textura_id.b, &normal_id.b,
                    &vertice_id.c, &textura_id.c, &normal_id.c);
            
            // uv, se supone que primmero guardamos los indices en un arreglo
            vertice_id.a_uv = nuevo.texturaUV[textura_id.a - 1];
            vertice_id.b_uv = nuevo.texturaUV[textura_id.b - 1];
            vertice_id.c_uv = nuevo.texturaUV[textura_id.c - 1];

            pushto_array(nuevo.indices, vertice_id);
            pushto_array(nuevo.n_indices, normal_id);
        }
        
        // textura
        else if((renglon[0] == 'v' && renglon[1] == 't') && (opt & UV) == UV)
        {
            // cargamos primero los indices de los uv en un arreglo
            // para despues rferenciar en los uv de cada vertica
            TexturaUV uv = {0};
            sscanf(renglon + 2, "%f %f", &uv.u, &uv.v); 
            pushto_array(nuevo.texturaUV, uv);
        }
    }

    nuevo.escala = (Vec3){{1.f, 1.f, 1.f}};
    nuevo.rotacion = (Vec3){{0.f, 0.f, 0.f}};
    nuevo.traslado = (Vec3){{0.f, 0.f, 0.f}};

    return nuevo;
}
