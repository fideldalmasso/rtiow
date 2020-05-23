#include<iostream>
#include "vec3.h"
#include "color.h"
#include "rayo.h"

using namespace std;

bool toca_la_esfera(const punto3& centro, double radio, const rayo &r){
	
	/*
	Queremos saber si un rayo toca una esfera.
	ecuacion del rayo: P = G + ut
	ecuacion de esfera: (x - C1)^2  + (y - C2)^2 + (z - C3)^2 = r^2

	si tomamos a (P - C) como un vector que va desde el centro de la esfera hasta su superficie
	ecuacion vectorial de la esfera:   (P - C) (P - C)   = r^2
	
	reemplazamos P por la ecuacion del rayo
	(G + ut - C) (G + ut - C) = r^2
	resolviendo algebraicamente
	[u u] t^2 + [2 b (G - C)] t + [(G - C) (G - C)] - r^2 = 0
	esto es una ecuacion cuadratica (que depende de t) con:
		a =  u u
		b = 2 b (G - C)
		c = (G - C) (G - C)
		discriminante = b b - 4 a c
	el rayo toca la esfera si el discriminante de la ecuacion es mayor a 0
	*/
	vec3 oc = r.origen() - centro; 
	auto a = producto_punto(r.direccion(),r.direccion());
	auto b = 2 * producto_punto(r.direccion(), oc);
	auto c = producto_punto(oc,oc) - radio*radio;
	auto discriminante = b*b - 4*a*c;
	return discriminante > 0;
}


//mezcla linealmente blanco y azul dependiendo del alto (coordenada y)
color color_de_rayo(const rayo& r){ 
	
	punto3 centro_esfera(0,0,-1);
	double radio_esfera = 0.5;
	if(toca_la_esfera(centro_esfera, radio_esfera, r))
	   return color(1,0,0);
	
	vec3 direccion_unitaria = vector_unitario(r.direccion());
	//en direccion_unitaria, la componente en y se mueve entre -1 y 1
	//pero necesito que se mueva entre 0 y 1, asi que 
	//mapeo [-1,1] a [0,1] de la siguiente forma
	auto t = 0.5 * (direccion_unitaria.y() + 1.0);
	
	//truco de interpolacion lineal o lerp (linear interpolation)
	// blendedValue = (1 - t) * startValue + t.endValue,  con t entre 0 y 1
	return (1.0-t)*color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
	// 1.0,1.0,1.0 es blanco
	// 0.5,0.7,1.0 es azul
}



int main() {
	//(1) rda = ancho / alto ->
	//(2) ancho = rda * alto
	//(3) alto = ancho / rda
	
	//uso(1)
	const auto relacion_de_aspecto = 16.0 / 9.0;
	const int ancho = 400;
	//uso (2)
	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
	
	cout << "P3\n" << ancho << ' ' << alto << "\n255\n";
	//uso (3)
	auto alto_viewport = 2.0;
	auto ancho_viewport = relacion_de_aspecto * alto_viewport;
	auto longitud_focal = 1.0;					//valores de z. Es la distancia de la camara al viewport
	
	auto origen = punto3(0,0,0);				
	auto horizontal = vec3(ancho_viewport,0,0); //valores de x. Es el ancho del viewport
	auto vertical = vec3(0,alto_viewport,0);	//valores de y. Es el alto del viewport
	auto esquina_inferior_izquierda = 	origen 
										- horizontal/2 
										- vertical/2 
										- vec3(0,0,longitud_focal);
	
	
	for (int j = alto -1; j >= 0; --j) {
		cerr << "\rScanlines remaining: " << j << ' ' << flush;
		for (int i = 0; i < ancho; ++i) {
			
			auto u = double(i) / (ancho - 1);
			auto v = double(j) / (alto - 1);
			
			//la direccion (vector AB) se obtiene como B-A
			//donde B es el punto que se va moviendo sobre el plano del viewport en cada iteracion
			rayo r(origen,    esquina_inferior_izquierda + u * horizontal + v * vertical
							- origen);
			
			color color_de_pixel = color_de_rayo(r);

			escribir_color(cout,color_de_pixel);
		}
	}
	cerr << "\nDone.\n";
}
