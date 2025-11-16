#pragma once

typedef union _vec2{
    float data[2];

    struct{
        float x;
        float y;
    }unpack;
}Vec2;

typedef union _vec3{
    float data[3];

    struct{
        float x;
        float y;
        float z;
    }unpack;
}Vec3;

typedef union _vec4{
	float data[4];

	struct{
		float x;
		float y;
		float z;
		float w;
	}unpack;
}Vec4;

float distanciav2(Vec2 p1, Vec2 p2);
Vec3 vec4_to_vec3(Vec4 *p);
Vec3 cross_vec3(Vec3 p, Vec3 v);
Vec3 resta_vec3(Vec3 p, Vec3 v);
Vec3 escala_vec3(Vec3 *p, float s);
float dot_vec3(Vec3 p, Vec3 v);

float magnitud(Vec3 p);
Vec3 normalizar_vec3(Vec3 p);
void normalizar_vec3_inplace(Vec3 *p);


void swap(Vec3 *v1, Vec3 *v2);


