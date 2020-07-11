#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtweekend.h"
#include <iostream>


void escribir_color(std::ostream &out, color pixel_color, int muestras_por_pixel){ // aca color es un alias de vec3!!!
	
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();
	
	auto scale = 1.0 / muestras_por_pixel;
	
	r*= scale;
	g*= scale;
	b*= scale;
	
	out << static_cast<int> (256 * clamp(r,0.0, 0.999)) << ' '
		<< static_cast<int> (256 * clamp(g,0.0, 0.999)) << ' '
		<< static_cast<int> (256 * clamp(b,0.0, 0.999)) << '\n';
}

#endif
