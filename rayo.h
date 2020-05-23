#ifndef RAYO_H
#define RAYO_H

#include "vec3.h"

class rayo {
public:
	rayo() {}
	rayo(const punto3& origen, const vec3& direccion) : 
		orig(origen), dir(direccion) {}
	
	punto3 origen() const { return orig;}
	vec3 direccion() const { return dir; }
	
	punto3 en(double t) const{
		return orig + (t*dir);
	}
		
public:
		punto3 orig;
		vec3 dir;
};

#endif

