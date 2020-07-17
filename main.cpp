#include "rtweekend.h"
#include "lista_chocable.h"
#include "esfera.h"
#include "color.h"
#include "camara.h"
#include "material.h"
#include<iostream>
#include <cstdio>
#include <ctime>
using namespace std;


color color_de_rayo(const rayo& r, const chocable& mundo, int profundidad){ 
	registro_choque registro;
	
	if(profundidad <= 0)
		return color(0,0,0);
	
	if(mundo.choca(r,0.001,infinito, registro)){
		//objetivo es un punto aleatorio dentro de la superficie de una esfera unitaria que 
		//es tangente a la otra esfera en el punto donde el rayo la toca
		rayo rayo_reflejado;
		color atenuacion;
		if(registro.material_ptr->refleja(r,registro,atenuacion,rayo_reflejado))
			//para mezclar los colores, los multiplico componente a componente
			//esto funciona porque las componentes siempre varían entre 0 y 1
			//entonces multiplicar dos colores siempre da como resultado otro color válido
			return atenuacion * color_de_rayo(rayo_reflejado,mundo, profundidad - 1);
		else return color(0,0,0);
	}
	
	//sino, dibujo el fondo
	//mezclar linealmente blanco y azul dependiendo del alto (coordenada y)
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
	freopen("../out.ppm", "w", stdout);
	//(1) rda = ancho / alto ->
	//(2) ancho = rda * alto
	//(3) alto = ancho / rda
	
	//uso(1)
	const auto relacion_de_aspecto = 16.0 / 9.0;
	const int ancho = 800;
	//uso (2)
	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
	const int muestras_por_pixel  = 100;
	const int profundidad_maxima = 50;
	
	cout << "P3\n" << ancho << ' ' << alto << "\n255\n";

	lista_chocable mundo;
	mundo.agregar(make_shared<esfera>(punto3(0,-100.5,-1), 100, make_shared<lambertiano>(color(0.5,0.2,0.2))));
	
	mundo.agregar(make_shared<esfera>(punto3(-1,0,-1), 0.5, make_shared<metalico>(color(0.8,0.8,0.8),0.0)));
	mundo.agregar(make_shared<esfera>(punto3(-0.4,-0.3,-0.6), -0.15, make_shared<dialectrico>(1.5)));
	mundo.agregar(make_shared<esfera>(punto3(0,0,-1), 0.5, make_shared<dialectrico>(2.1)));
	mundo.agregar(make_shared<esfera>(punto3(0.4,-0.3,-0.6), 0.15, make_shared<metalico>(color(1.0,1.0,0.0),0.2)));
	mundo.agregar(make_shared<esfera>(punto3(1,0,-1), 0.5, make_shared<lambertiano>(color(0.9,0.9,0.3))));
	
	
	camara cam(110,double(ancho)/alto);
	
	time_t inicio,fin;
	time(&inicio);
	for (int j = alto -1; j >= 0; --j) {
		cerr << "\rScanlines remaining: " << j << ' ' << flush;
		for (int i = 0; i < ancho; ++i) {
			color pixel_color(0,0,0);
			
			
			//el color de cada pixel es el promedio de los colores de ese pixel 
			//junto con los colores de los pixeles que lo rodean.
			//aqui solo se acumula la suma, pero en escribir_color, se divide
			//por la cantidad de muestras
			for(int s = 0; s < muestras_por_pixel; ++s){
				auto u = (i + double_aleatorio()) / (ancho-1);
				auto v = (j + double_aleatorio()) / (alto-1);
				rayo r = cam.get_rayo(u,v);
				pixel_color += color_de_rayo(r,mundo, profundidad_maxima);
			}
			escribir_color(cout,pixel_color, muestras_por_pixel);
		}
	}
	time(&fin);
	double tiempo_transcurrido = (double) fin - inicio;
	fprintf(stderr,"\nTiempo transcurrido: %.1f",tiempo_transcurrido);
	cerr << "\nDone.\n";
}
