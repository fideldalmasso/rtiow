#ifndef CAMARA_H
#define CAMARA_H

#include "rtweekend.h"

class camara {
public:
	camara(punto3 mirar_desde, punto3 mirar_hacia, vec3 v_arriba, double fov_vertical, double relacion_de_aspecto) {
		//(1) rda = ancho / alto ->
		//(2) ancho = rda * alto
		//(3) alto = ancho / rda
		
		auto tita = grados_a_radianes(fov_vertical);
		auto h = tan(tita/2);
		
		auto alto_viewport = 2.0 * h;
		auto ancho_viewport = relacion_de_aspecto * alto_viewport;
		
		//auto longitud_focal = 1.0;					//valores de z. Es la distancia de la camara al viewport
		
		//la camara es un plano definido por tres vectores ortogonales u,v,w
		auto w = vector_unitario(mirar_desde - mirar_hacia); //es un vector que apunta en direccion contraria hacia donde estoy viendo
		auto u = vector_unitario(producto_cruz(v_arriba,w)); //es el vector que apunta hacia la derecha
		auto v = producto_cruz(w,u); //es el vector que apunta hacia arriba
		
		origen = mirar_desde;				
		horizontal = ancho_viewport * u; //valores de x. Es el ancho del viewport
		vertical = alto_viewport * v;	//valores de y. Es el alto del viewport
		
		esquina_inferior_izquierda = 	origen - horizontal/2 - vertical/2 - w;
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
