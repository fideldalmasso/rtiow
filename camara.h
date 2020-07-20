#ifndef CAMARA_H
#define CAMARA_H

#include "rtweekend.h"

class camara {
public:
	camara(){}
	camara(punto3 mirar_desde, 
		   punto3 mirar_hacia, 
		   vec3 v_arriba, 
		   double fov_vertical, 
		   double relacion_de_aspecto,
		   double apertura,
		   double distancia_focal) {
		//(1) rda = ancho / alto ->
		//(2) ancho = rda * alto
		//(3) alto = ancho / rda
		
		auto tita = grados_a_radianes(fov_vertical);
		auto h = tan(tita/2);
		
		auto alto_viewport = 2.0 * h;
		auto ancho_viewport = relacion_de_aspecto * alto_viewport;
		
		//la camara es un plano definido por tres vectores ortogonales u,v,w
		w = vector_unitario(mirar_desde - mirar_hacia); //es un vector que apunta en direccion contraria hacia donde estoy viendo
		u = vector_unitario(producto_cruz(v_arriba,w)); //es el vector que apunta hacia la derecha
		v = producto_cruz(w,u); //es el vector que apunta hacia arriba
		
		origen = mirar_desde;				
		horizontal = distancia_focal * ancho_viewport * u; //valores de x. Es el ancho del viewport
		vertical = distancia_focal * alto_viewport * v;	//valores de y. Es el alto del viewport
		esquina_inferior_izquierda = origen - horizontal/2 - vertical/2 - (distancia_focal * w);
		radio_lente = apertura/2;
	}
	rayo get_rayo(double s, double t) const{
		vec3 punto_aleatorio = radio_lente * vector_en_disco_unitario_aleatorio();
		vec3 margen = u * punto_aleatorio.x() + v * punto_aleatorio.y();
		
		return rayo(origen + margen, esquina_inferior_izquierda + s * horizontal + t * vertical - origen - margen);
	}
	
private:
		punto3 origen;
		punto3 esquina_inferior_izquierda;
		vec3 horizontal;
		vec3 vertical;
		vec3 u,v,w;
		double radio_lente;
};




#endif
