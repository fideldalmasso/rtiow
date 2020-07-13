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
	
	r= sqrt(scale * r);
	g= sqrt(scale * g);
	b= sqrt(scale * b);
	
	auto rtemp = static_cast<int> (256 * clamp(r,0.0, 0.999));
	auto gtemp = static_cast<int> (256 * clamp(g,0.0, 0.999));
	auto btemp = static_cast<int> (256 * clamp(b,0.0, 0.999));
	
	rtemp < 256 || rtemp > 0 ? r = rtemp : r = 0.0;
	gtemp < 256 || gtemp > 0? g = gtemp : g = 0.0;
	btemp < 256 || btemp > 0? b = btemp : b = 0.0;

	
	out << r << ' '
		<< g << ' '
		<< b<< '\n';
}

#endif
