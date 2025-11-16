#include "luz.h"
#include <stdint.h>

uint32_t luz_intensidad(uint32_t color, float porcentaje){
	if (porcentaje < 0) porcentaje = 0.f;
	if (porcentaje > 1) porcentaje = 1.f;

	// Extraer colores
	uint32_t r = (color & 0xFF000000) * porcentaje;
	uint32_t g = (color & 0x00FF0000) * porcentaje;
	uint32_t b = (color & 0x0000FF00) * porcentaje;
	uint32_t a = (color & 0x000000FF);

	return (r & 0xFF000000) |
	       (g & 0x00FF0000) |
	       (b & 0x0000FF00) |
	       (a);
}
