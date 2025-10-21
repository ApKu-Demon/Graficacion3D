#include "matrix.h"
#include <stdio.h>

Mat4 mat4_eye(void)
{

    /*
    [1, 0, 0, 0]
    [0, 1, 0, 0]
    [0, 0, 1, 0]
    [0, 0, 0, 1]
    */

    Mat4 identidad = {0};
    identidad.data[0] = 1;
    identidad.data[5] = 1;
    identidad.data[10] = 1;
    identidad.data[15] = 1;
    
    return identidad;
}

void mat4_push_escala(Mat4 *matriz, Vec3 escala)
{
    /*
    [1, sx, 0, 0]
    [sy, 1, 0, 0]
    [0,  0, 1, 0]
    [0,  0, 0, 1]
    */
    
    matriz->data[0] = escala.unpack.x;
    matriz->data[5] = escala.unpack.y;
    matriz->data[10] = escala.unpack.z;
}

void mat4_push_traslado(Mat4 *matriz, Vec3 pos)
{
    /*
    [1, 0, 0, x]
    [0, 1, 0, y]
    [0, 0, 1, z]
    [0, 0, 0, 1]
    */
    
    matriz->data[3] = pos.unpack.x;
    matriz->data[7] = pos.unpack.y;
    matriz->data[11] = pos.unpack.z;
}

void mat4_push_rotar_x(Mat4 *matriz, float radianes)
{
    /*
    [cosr,      0,      sinr,       0]
    [0,         1,      0,          0]
    [-sinr,     0,      cosr,       0]
    [0,         0,      0,          1]
    */

    // PITCH

    matriz->data[0] = cos(radianes);
    matriz->data[2] = sin(radianes);
    matriz->data[8] = -sin(radianes);
    matriz->data[10] = cos(radianes);
}

void mat4_push_rotar_y(Mat4 *matriz, float radianes)
{
    /*
    [cosr,      -sinr,      0,      0]
    [sinr,      cosr,       0,      0]
    [0,         0,          0,      0]
    [0,         0,          0,      1]
    */

    // YAW

    matriz->data[0] = cos(radianes);
    matriz->data[1] = -sin(radianes);
    matriz->data[4] = sin(radianes);
    matriz->data[5] = cos(radianes);
}

void mat4_push_rotar_z(Mat4 *matriz, float radianes)
{
    /*
    [cosr,      -sinr,      0,      0]
    [0,      cosr,       0,      0]
    [sinr,      0,          0,      0]
    [0,         0,          0,      1]
    */

    // YAW

    matriz->data[5] = cos(radianes);
    matriz->data[6] = -sin(radianes);
    matriz->data[9] = sin(radianes);
    matriz->data[10] = cos(radianes);

}

void mat4_push_rotar(Mat4 *matriz, Vec3 rotacion)
{
    // rotacion z, rotacion y, rotacion x

    /*
    [sx*cosa*cosb,      cosa*sinb*sins - sina*coss,         cosa*sinb*coss + sina*sins,         0]
    [sina*cosb,         sy*sina*sinb*sins + cosa*coss,      sina*sinb*coss - cosa*sins,        0]
    [-sinb,             cosb*sins,                          sz*cosb*coss,      0]
    [0,         0,          0,      1]
    */

    float cosa = cos(rotacion.unpack.z), sina = sin(rotacion.unpack.z);
    float cosb = cos(rotacion.unpack.y), sinb = sin(rotacion.unpack.y);
    float coss = cos(rotacion.unpack.x), sins = sin(rotacion.unpack.x);

    matriz->data[0] *= cosa * cosb;
    matriz->data[1] = cosa * sinb * sins - sina * coss;
    matriz->data[2] = cosa * sinb * coss + sina * sins;
    matriz->data[4] = sina * cosb;
    matriz->data[5] *= sina * sinb * sins + cosa * coss;
    matriz->data[6] = sina * sinb * coss - cosa * sins;
    matriz->data[8] = -sinb;
    matriz->data[9] = cosb * sins;
    matriz->data[10] *= cosb * coss;
}

Vec4 mat4_dot_vec4(Mat4 *matriz, Vec4 *punto)
{
    Vec4 c;

    /*
    [a1, b1, c1, d1]        [x]
    [a2, b2, c2, d2]    .   [y]     =   [a2*x + b2*y + c2*z + d2*1]
    [a3, b3, c3, d3]        [z]
    [a4, b4, c4, d4]        [1]
    */

    c.unpack.x = matriz->data[0]*punto->unpack.x +
                 matriz->data[1]*punto->unpack.y +
                 matriz->data[2]*punto->unpack.z +
                 matriz->data[3]*punto->unpack.w;

    c.unpack.y = matriz->data[4]*punto->unpack.x +
                 matriz->data[5]*punto->unpack.y +
                 matriz->data[6]*punto->unpack.z +
                 matriz->data[7]*punto->unpack.w;

    c.unpack.z = matriz->data[8]*punto->unpack.x +
                 matriz->data[9]*punto->unpack.y +
                 matriz->data[10]*punto->unpack.z +
                 matriz->data[11]*punto->unpack.w;

    c.unpack.w = matriz->data[12]*punto->unpack.x +
                 matriz->data[13]*punto->unpack.y +
                 matriz->data[14]*punto->unpack.z +
                 matriz->data[15]*punto->unpack.w;

    return c;
}

Mat4 mat4_matriz_proyeccion(float fov, float aspect, float znear, float zfar)
{
    // regresa la matriz de proyeccion
    /*
        [(h/w)*1/tan(fov/2),        0       ,       0       ,       0       ]
        [       0           ,  1/tan(fov/2) ,       0       ,       0       ]
        [       0           ,       0       ,   zf/(zf-zn)  ,       0       ]
        [       0           ,       0       ,       1       ,       0       ]

        aspect = (h/w) monitor normal o (w/h) celulares donde la altura es mayor que lo largo
    */

    Mat4 p = {0};
    float inv_ang = 1/tan(fov/2);

    p.data[0] = aspect*inv_ang;
    p.data[5] = inv_ang;
    p.data[10] = zfar/(zfar-znear);
    p.data[11] = (-zfar*znear)/(zfar-znear);
    p.data[14] = 1.f;
    
    return p;
}

Vec2 proyeccion_ortografica(Vec3 punto, float fovf)
{
	Vec2 p = {{
		punto.unpack.x * fovf,
		punto.unpack.y * fovf
	}};
	return p;
}

Vec2 proyeccion_perspectiva(Vec3 punto, float fovf)
{
	Vec2 p = {{
		fovf * punto.unpack.x / punto.unpack.z,
		fovf * punto.unpack.y / punto.unpack.z
	}};
	return p;
}

Vec4 proyeccion(Mat4 *mat, Vec4 p)
{
    Vec4 r = mat4_dot_vec4(mat, &p);
    if(r.unpack.w!=0.0)
    {
        r.unpack.x/=r.unpack.w;
        r.unpack.y/=r.unpack.w;
        r.unpack.z/=r.unpack.w;
    }
    return r;
}


void print_matriz(Mat4 *m)
{
    for(int i=0; i<4; i++)
    {
        printf("[");
        for(int j=0; j<4; j++)
        {
            char coma = (j<3)?',':'\0';
            printf("%f%c", m->data[i * 4 + j], coma);
        }
        printf("]\n");
    }
}

