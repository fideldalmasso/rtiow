#ifndef RAYO_H
#define RAYO_H

#include "vec3.h"

class rayo {
public:
	rayo() {}
	rayo(const punto3& origen, const vec3& direccion, double tiempo = 0.0) : 
		orig(origen), dir(direccion), tm(tiempo) {}
	
	punto3 origen() const { return orig;}
	vec3 direccion() const { return dir; }
	double tiempo() const { return tm; }
	
	punto3 en(double t) const{
		return orig + (t*dir);
	}
	
public:
		punto3 orig;
		vec3 dir;
		double tm;
};

#endif

