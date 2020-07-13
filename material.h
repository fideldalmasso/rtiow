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
	metalico(const color& a, double f) : albedo(a), aspereza(f < 1 ? f : 1) {}
	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const{
		vec3 direccion_reflejada = reflejar(vector_unitario(rayo_incidente.direccion()), registro.normal);
		rayo_reflejado = rayo(registro.p, direccion_reflejada + aspereza * vector_en_esfera_unitaria_aleatorio());
		atenuacion = albedo;
		//solo reflejar cuando el rayo saliente está del mismo lado que el que ingresa
		return (producto_punto(rayo_reflejado.direccion(), registro.normal) > 0); 
	}
	
public:
	color albedo;
	double aspereza;
};

class dialectrico : public material{
public:
	dialectrico(double ir) : indice_de_refraccion(ir) {}
	
	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const{
	atenuacion = color(1.0,1.0,1.0);
	double indice1_sobre_indice2;
	if(registro.cara_frontal)
		indice1_sobre_indice2 = 1.0 / indice_de_refraccion;
	else
		indice1_sobre_indice2 = indice_de_refraccion;
	
	vec3 direccion_unitaria = vector_unitario(rayo_incidente.direccion());
	vec3 direccion_refractada = refractar(direccion_unitaria, registro.normal, indice1_sobre_indice2);
	rayo_reflejado = rayo(registro.p, direccion_refractada);
	return true;
	}
public:
	double indice_de_refraccion;
};



#endif
