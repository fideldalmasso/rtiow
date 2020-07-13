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

class metalico : public material {
public:
	metalico(const color& a) : albedo(a) {}
	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const{
		vec3 direccion_reflejada = reflejar(vector_unitario(rayo_incidente.direccion()), registro.normal);
		rayo_reflejado = rayo(registro.p, direccion_reflejada);
		atenuacion = albedo;
		//solo reflejar cuando el rayo saliente está del mismo lado que el que ingresa
		return (producto_punto(rayo_reflejado.direccion(), registro.normal) > 0); 
	}
	
public:
	color albedo;
};




#endif
