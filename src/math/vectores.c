#include "vectores.h"
#include <math.h>

float distanciav2(Vec2 p1, Vec2 p2){
    return sqrt((p2.unpack.x - p1.unpack.x) * (p2.unpack.x - p1.unpack.x))
                +
                (p2.unpack.y - p1.unpack.y) * (p2.unpack.y - p1.unpack.y);
}

Vec3 vec4_to_vec3(Vec4 *p)
{
	return (Vec3){{p->unpack.x, p->unpack.y, p->unpack.z}};
}

Vec4 vec3_to_vec4(Vec3 *p)
{
	return (Vec4){{p->unpack.x, p->unpack.y, p->unpack.z, 1.f}};
}

Vec3 cross_vec3(Vec3 p, Vec3 v){
	return (Vec3){{
		p.unpack.y * v.unpack.z - v.unpack.y * p.unpack.z,		
		p.unpack.z * v.unpack.x - p.unpack.x * v.unpack.z,		
		p.unpack.x * v.unpack.y - p.unpack.y * v.unpack.x
	}};
}

Vec3 resta_vec3(Vec3 p, Vec3 v){
	return (Vec3){{
		p.unpack.x - v.unpack.x,
		p.unpack.y - v.unpack.y,
		p.unpack.z - v.unpack.z,
	}};
}

float dot_vec3(Vec3 p, Vec3 v){
	return p.unpack.x * v.unpack.x + p.unpack.y * v.unpack.y + p.unpack.z * v.unpack.z;
}

float magnitud(Vec3 p){
	return sqrt(pow(p.unpack.x, 2) + pow(p.unpack.y, 2)) + pow(p.unpack.z, 2);
}

Vec3 normalizar_vec3(Vec3 p){
	float mag = magnitud(p);

	return (Vec3){{
		p.unpack.x / mag,
		p.unpack.y / mag,
		p.unpack.z / mag,
	}};
}

void normalizar_vec3_inplace(Vec3 *p){
	float mag = magnitud(*p);

	p -> unpack.x /= mag;
	p -> unpack.y /= mag;
	p -> unpack.z /= mag;
}

Vec3 escala_vec3(Vec3 *p, float s){
	return (Vec3){{
		p -> unpack.x * s,
		p -> unpack.y * s,
		p -> unpack.z * s,
	}};
}

void swap(Vec3 *v1, Vec3 *v2)
{
    Vec3 temp = *v1; 
    
    *v1 = *v2; 
    *v2 = temp;
}


