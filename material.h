#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"
#include "textura.h"

double schlick(double coseno, double indice_de_refraccion){
	auto r0 = (1 - indice_de_refraccion) / (1 + indice_de_refraccion);
	r0 = r0 * r0;
	return r0 + (1-r0)*pow((1 - coseno),5);
}

class material{
public:

	virtual color emitido(double u, double v, const punto3& p) const{
		return color(0,0,0);
	}

	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const = 0;
};

class lambertiano : public material {
public:
	lambertiano(const color& a) : albedo(make_shared<color_solido>(a)) {}
	lambertiano(shared_ptr<textura> a): albedo(a){}
	
	// lambertiano(const color&a) : albedo(a) {}


	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const{
		vec3 direccion_reflejada  = registro.normal + vector_unitario_aleatorio();
		rayo_reflejado = rayo(registro.p, direccion_reflejada, rayo_incidente.tiempo());
		atenuacion = albedo->valor(registro.u,registro.v,registro.p);
		return true;
	}
public:
	// color albedo;
	shared_ptr<textura> albedo;
	
};

class metalico : public material {
public:
	metalico(const color& a, double f) : albedo(a), aspereza(f < 1 ? f : 1) {}
	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const{
		vec3 direccion_reflejada = reflejar(vector_unitario(rayo_incidente.direccion()), registro.normal);
		rayo_reflejado = rayo(registro.p, direccion_reflejada + aspereza * vector_en_esfera_unitaria_aleatorio(), rayo_incidente.tiempo());
		atenuacion = albedo;
		//solo reflejar cuando el rayo saliente est� del mismo lado que el que ingresa
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
	double indice1_sobre_indice2 = (registro.cara_frontal) ? (1.0 / indice_de_refraccion) : (indice_de_refraccion);
	vec3 direccion_unitaria = vector_unitario(rayo_incidente.direccion());
	double cos_tita = fmin(producto_punto(-direccion_unitaria, registro.normal),1.0);
	double sin_tita = sqrt(1.0 - cos_tita*cos_tita);
	
	if(indice1_sobre_indice2 * sin_tita > 1.0){ 
		//hacemos una reflexion interna total, porque la ley de Snell no funciona en este caso
		vec3 direccion_reflejada = reflejar(direccion_unitaria,registro.normal);
		rayo_reflejado = rayo(registro.p, direccion_reflejada, rayo_incidente.tiempo());
		return true;	
	}
	else {
		double probabilidad_de_reflexion = schlick(cos_tita, indice1_sobre_indice2);
		if(double_aleatorio() < probabilidad_de_reflexion){
			//reflejar de todas formas
			vec3 direccion_reflejada = reflejar(direccion_unitaria,registro.normal);
			rayo_reflejado = rayo(registro.p, direccion_reflejada, rayo_incidente.tiempo());
			return true;
		}
		else{
		   //refractar
		vec3 direccion_refractada = refractar(direccion_unitaria, registro.normal, indice1_sobre_indice2);	
		rayo_reflejado = rayo(registro.p, direccion_refractada, rayo_incidente.tiempo());
		return true;
		}
	}
	}
public:
	double indice_de_refraccion;
};

class luz_difusa : public material{
public:
	luz_difusa(shared_ptr<textura> a): emitir(a){}
	
	luz_difusa(color c):emitir(make_shared<color_solido>(c)){}

	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const override{
		return false;
	}

	virtual color emitido(double u, double v, const punto3& p) const override {
		return emitir->valor(u,v,p);
	}

public:
	shared_ptr<textura> emitir;

};

class isotropico : public material{
public:
	isotropico(color c): albedo(make_shared<color_solido>(c)){}
	isotropico(shared_ptr<textura> t): albedo(t){}

	virtual bool refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const override{
		rayo_reflejado = rayo(registro.p,vector_en_esfera_unitaria_aleatorio(),rayo_incidente.tiempo());
		atenuacion = albedo->valor(registro.u,registro.v,registro.p);
		return true;
	}

public:
	shared_ptr<textura> albedo;
};


#endif
