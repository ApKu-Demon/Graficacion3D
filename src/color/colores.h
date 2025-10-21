#pragma once
#include <stdint.h>

typedef union _color
{
	uint32_t hex;
	struct
	{
		uint8_t a;
		uint8_t b;
		uint8_t g;
		uint8_t r;
	}rgba;
}Color;

Color lerp_color(Color a, Color b, float t);


