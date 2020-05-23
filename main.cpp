#include<iostream>
#include "vec3.h"
#include "color.h"
#include "rayo.h"

using namespace std;

//mezcla linealmente blanco y azul dependiendo del alto (coordenada y)
color color_de_rayo(const rayo& r){ 
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
