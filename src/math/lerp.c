#include "vectores.h"
#include "lerp.h"

Vec2 lerpv2(Vec2 p1, Vec2 p2, float t)
{
	Vec2 nuevo;

	nuevo.unpack.x = p1.unpack.x + (p2.unpack.x -p1.unpack.x) * t;
	nuevo.unpack.y = p1.unpack.y + (p2.unpack.y -p1.unpack.y) * t;

	return nuevo;
}


Vec2 lerp2v2(Vec2 p1, Vec2 p2, Vec2 p3, float t)
{
	return lerpv2(lerpv2(p1, p2, t), lerpv2(p2, p3, t), t);
}



