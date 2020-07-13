#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"
class material{
public:
	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const = 0;
};

class lambertiano : public material {
public:
	lambertiano(const color&a) : albedo(a) {}
	
	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const{
		vec3 direccion_reflejada  = registro.normal + vector_unitario_aleatorio();
		rayo_reflejado = rayo(registro.p, direccion_reflejada);
		atenuacion = albedo;
		return true;
	}
public:
	color albedo;
	
};



#endif
