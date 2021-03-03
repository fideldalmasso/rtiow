#ifndef ESFERA_EN_MOVIMIENTO_H
#define ESFERA_EN_MOVIMIENTO_H

#include "aabb.h"
#include "rtweekend.h"
#include "chocable.h"

class esfera_en_movimiento  : public chocable {
public:
	esfera_en_movimiento() {}
	esfera_en_movimiento(punto3 cen0, 
						 punto3 cen1, 
						 double _tiempo0, 
						 double _tiempo1, 
						 double r, 
						 shared_ptr<material> m): centro0(cen0),centro1(cen1),tiempo0(_tiempo0),tiempo1(_tiempo1),radio(r),material_ptr(m) {};

	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;
	virtual bool caja_delimitadora(double _tiempo0, double _tiempo1, aabb& caja_saliente) const override;
	
	punto3 centro(double tiempo) const;
	
public:
		punto3 centro0, centro1;
		double tiempo0, tiempo1;
		double radio;
		shared_ptr<material> material_ptr;
			
};

punto3 esfera_en_movimiento::centro(double tiempo) const {
	return centro0 + ((tiempo - tiempo0) / (tiempo1 - tiempo0))*(centro1 - centro0);
}

bool esfera_en_movimiento::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{
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
		vec3 co = r.origen() - centro(r.tiempo()); 
		vec3 u = r.direccion();
		auto a = u.longitud_cuadrada();	
		auto medio_b = producto_punto(co,u);
		auto c = co.longitud_cuadrada() - radio*radio;
		auto discriminante = medio_b*medio_b - a*c;
		
		if(discriminante>0){ //el rayo toca la esfera
			auto raiz = sqrt(discriminante);
			auto temp = (-medio_b - raiz) / a; //formula resolvente
			
			if(temp < t_max && temp > t_min){ //pruebo la formula con -
				registro.t = temp;
				registro.p = r.en(registro.t);
				
				vec3 normal_saliente = (registro.p - centro(r.tiempo())) / radio;
				registro.set_cara_y_normal(r,normal_saliente);
				registro.material_ptr = material_ptr;
				return true;
			}
			temp = (-medio_b + raiz) / a;
			
			if(temp < t_max && temp > t_min){ //pruebo la formula con +
				registro.t = temp;
				registro.p = r.en(registro.t);
				vec3 normal_saliente = (registro.p - centro(r.tiempo())) / radio;
				registro.set_cara_y_normal(r,normal_saliente);
				registro.material_ptr = material_ptr;
				return true;
			}
		}
		return false; //el rayo no toca la esfera
		
	}


bool esfera_en_movimiento::caja_delimitadora(double _tiempo0, double _tiempo1, aabb& caja_saliente) const{
	aabb caja0( centro(_tiempo0)- vec3(radio,radio,radio), centro(_tiempo0) + vec3(radio,radio,radio));
	aabb caja1( centro(_tiempo1)- vec3(radio,radio,radio), centro(_tiempo1) + vec3(radio,radio,radio));
	caja_saliente = caja_agrupadora(caja0,caja1);
	return true;
}

#endif

