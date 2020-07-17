#ifndef CAMARA_H
#define CAMARA_H

#include "rtweekend.h"

class camara {
public:
	camara(double fov_vertical, double ra) {
		//(1) rda = ancho / alto ->
		//(2) ancho = rda * alto
		//(3) alto = ancho / rda
		
		auto tita = grados_a_radianes(fov_vertical);
		auto h = tan(tita/2);
		
		auto relacion_de_aspecto = 16.0 / 9.0;
		auto alto_viewport = 2.0 * h;
		auto ancho_viewport = relacion_de_aspecto * alto_viewport;
		
		auto longitud_focal = 1.0;					//valores de z. Es la distancia de la camara al viewport
		
		origen = punto3(0,0,0);				
		horizontal = vec3(ancho_viewport,0.0,0.0); //valores de x. Es el ancho del viewport
		vertical = vec3(0.0,alto_viewport,0.0);	//valores de y. Es el alto del viewport
	
		esquina_inferior_izquierda = 	origen 
			- horizontal/2 
			- vertical/2 
			- vec3(0,0,longitud_focal);
	}
	rayo get_rayo(double u, double v) const{
		return rayo(origen, esquina_inferior_izquierda + u * horizontal + v * vertical - origen);
	}
	
private:
		punto3 origen;
		punto3 esquina_inferior_izquierda;
		vec3 horizontal;
		vec3 vertical;
	
};




#endif
