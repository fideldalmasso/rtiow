#ifndef CHOCABLE_H
#define CHOCABLE_H

#include "rayo.h"

struct registro_choque{
	punto3 p;
	vec3 normal;
	double t;

	
};


class chocable {
public:
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& rec) const = 0;
};

#endif

