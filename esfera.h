#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"

class esfera: public chocable {
public:
	esfera() {}
	esfera(punto3 cen, double r, shared_ptr<material> m) : centro(cen), radio(r), material_ptr(m) {};
	
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override;
	
public:
	punto3 centro;
	double radio;
	shared_ptr<material> material_ptr;

private:
	static void get_uv_esfera(const punto3& p, double& u, double& v){
		auto tita = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;

		u = phi/ (2*pi);
		v = tita/pi;
	}
};


bool esfera::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{
	/*
	Queremos saber si un rayo toca una esfera.
	ecuacion del rayo: P = O + ut
	ecuacion de esfera: (x - C1)^2  + (y - C2)^2 + (z - C3)^2 = r^2
	
	si tomamos a (P - C) como un vector que va desde el centro de la esfera hasta su superficie
	ecuacion vectorial de la esfera:   (P - C) (P - C)   = r^2
	
	reemplazamos P por la ecuacion del rayo
	(O + ut - C) (O + ut - C) = r^2
	resolviendo algebraicamente
	[u u] t^2 + [2 u (O - C)] t + [(O - C) (O - C)] - r^2 = 0
	esto es una ecuacion cuadratica (que depende de t) con:
	
	a =  u . u
	b = 2 u . (O - C) =	2 u . co
	c = (O - C) . (O - C) = co . co
	discriminante = b b - 4 a c
	
	el rayo toca la esfera si el discriminante de la ecuacion es mayor a 0
	
	Calculo original:
	auto a = producto_punto(u,u);
	auto b = 2 * producto_punto(co,u);
	auto c = producto_punto(co,co) - radio*radio;
	auto discriminante = b*b - 4*a*c;
	*/
	//Calculo simplificado:
	vec3 co = r.origen() - centro; 
	vec3 u = r.direccion();
	auto a = u.longitud_cuadrada();	
	auto medio_b = producto_punto(co,u);
	auto c = co.longitud_cuadrada() - radio*radio;
	auto discriminante = medio_b*medio_b - a*c;

	if(discriminante<0) return false; //el rayo NO toca la esfera

	auto raiz_discriminante = sqrt(discriminante);

	auto raiz = (-medio_b - raiz_discriminante) / a; //formula resolvente con menos
	
	if(raiz < t_min || raiz > t_max){
		
		raiz = (-medio_b + raiz_discriminante) / a;  //formula resolvente con mas
		
		if(raiz < t_min || raiz> t_max) return false;  //el rayo NO toca la esfera dentro del tiempo indicado
	}

	registro.t = raiz;
	registro.p = r.en(registro.t);

	vec3 normal_saliente = (registro.p - centro) / radio;
	registro.set_cara_y_normal(r,normal_saliente);
	get_uv_esfera(normal_saliente, registro.u, registro.v);
	registro.material_ptr = material_ptr;

	return true;
	
}

bool esfera::caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const{
	caja_saliente = aabb( centro- vec3(radio,radio,radio), centro + vec3(radio,radio,radio));
	return true;
}


#endif

