#ifndef CHOCABLE_H
#define CHOCABLE_H

#include "rtweekend.h"
#include "aabb.h"

class material; //no se para que se usa, pero supuestamente el puntero que se usa mas abajo apunta a una clase.

struct registro_choque{
	punto3 p;
	vec3 normal;
	shared_ptr<material> material_ptr;
	double t;
	bool cara_frontal;
	
	inline void set_cara_y_normal(const rayo& r, const vec3& normal_saliente){

		// es true cuando el rayo y la normal estan del mismo lado
		cara_frontal = producto_punto(r.direccion(),normal_saliente) < 0; 
		// si estan del mismo lado, la normal es igual a la saliente, sino es la contraria
		normal = cara_frontal ? normal_saliente : -normal_saliente;
	}
	
};


class chocable {
public:
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& rec) const = 0;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const = 0;
};

#endif

