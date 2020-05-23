#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using namespace std;

void escribir_color(ostream &out, color pixel_color){ // aca color es un alias de vec3!!!
	out << static_cast<int> (255.999 * pixel_color.x()) << ' '
		<< static_cast<int> (255.999 * pixel_color.y()) << ' '
		<< static_cast<int> (255.999 * pixel_color.z()) << '\n';
}

#endif
