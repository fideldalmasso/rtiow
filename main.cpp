#include<iostream>
#include "vec3.h"
#include "color.h"
#include "rayo.h"
#include "chocable.h"

using namespace std;

double toca_la_esfera(const punto3& centro, double radio, const rayo& r){
	
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
	
	if(discriminante<0){
		return -1.0;
	}
	else{
		//retorna el primer valor de t para el que el rayo toca la esfera
		//t es siempre un valor positivo en este caso
		//return (-b - sqrt(discriminante)) / (2.0 * a); retorno anterior
		return (-medio_b -sqrt(discriminante)) / a;
	}
}



color color_de_rayo(const rayo& r){ 
	
	
	punto3 centro_esfera(0,0,-1);
	double radio_esfera = 0.5;
	auto t = toca_la_esfera(centro_esfera, radio_esfera, r);
	
	if(t > 0.0){ //el rayo toca la esfera
		
		//n es el vector normal de la esfera: n = P - C
		vec3 n = vector_unitario(r.en(t) - centro_esfera);
		// n es un vector unitario y todas sus componentes varian entre -1 y 1
		// entonces tenemos que mapear sus componentes a [0,1]
		return 0.5*color(n.x()+1, n.y()+1, n.z()+1);
		
	}
	
	//sino, dibujo el fondo
	//mezclar linealmente blanco y azul dependiendo del alto (coordenada y)
	vec3 direccion_unitaria = vector_unitario(r.direccion());
	//en direccion_unitaria, la componente en y se mueve entre -1 y 1
	//pero necesito que se mueva entre 0 y 1, asi que 
	//mapeo [-1,1] a [0,1] de la siguiente forma
	t = 0.5 * (direccion_unitaria.y() + 1.0);
	
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
	auto longitud_fcoal = 1.0;					//valores de z. Es la distancia de la camara al viewport
	
	auto origen = punto3(0,0,0);				
	auto horizontal = vec3(ancho_viewport,0,0); //valores de x. Es el ancho del viewport
	auto vertical = vec3(0,alto_viewport,0);	//valores de y. Es el alto del viewport
	auto esquina_inferior_izquierda = 	origen 
										- horizontal/2 
										- vertical/2 
										- vec3(0,0,longitud_fcoal);
	
	
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
