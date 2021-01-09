#include "rtweekend.h"
#include "lista_chocable.h"
#include "esfera.h"
#include "color.h"
#include "camara.h"
#include "material.h"
#include<iostream>
#include <cstdio>
#include <ctime>
#include "esfera_en_movimiento.h"
using namespace std;

lista_chocable escena_aleatoria(){
	lista_chocable mundo;
	auto material_suelo = make_shared<lambertiano>(color(0.5,0.5,0.5));
	mundo.agregar(make_shared<esfera>(punto3(0,-1000,0),1000,material_suelo));

	for(int x = -11; x<11; x++){
		for(int z = -11; z<11; z++){
			auto elegir_material = double_aleatorio();
			auto radio  = double_aleatorio(0.1,0.4);
			punto3 centro(x + 0.9 * double_aleatorio(),radio, z + 0.9*double_aleatorio());

			shared_ptr<material> material_esfera;

			if(elegir_material < 0.8){
				//difuso
				auto albedo = color::aleatorio() * color::aleatorio();
				material_esfera = make_shared<lambertiano>(albedo);
				auto centro2 = centro + vec3(0,double_aleatorio(0,0.5),0);
				mundo.agregar(make_shared<esfera_en_movimiento>(centro,centro2,0.0,1.0,0.2,material_esfera));
			}
			else if(elegir_material < 0.95){
				//metal
				auto albedo = color::aleatorio(0.5,1);
				auto aspereza = double_aleatorio(0,0.5);
				material_esfera = make_shared<metalico>(albedo,aspereza);
				mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
			}
			else{
				//vidrio
				material_esfera = make_shared<dialectrico>(1.5);
				mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
			
			}


		}
	}
	auto material1 = make_shared<dialectrico>(1.5);
	auto material2 = make_shared<lambertiano>(color(0.4,0.2,0.1));
	auto material3 = make_shared<metalico>(color(0.7,0.6,0.5),0.0);

	mundo.agregar(make_shared<esfera>(punto3(0,1,0),1.0,material1));
	mundo.agregar(make_shared<esfera>(punto3(-4,1,0),1.0,material2));
	mundo.agregar(make_shared<esfera>(punto3(4,1,0),1.0,material3));


	return mundo;

}

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
			//esto funciona porque las componentes siempre var�an entre 0 y 1
			//entonces multiplicar dos colores siempre da como resultado otro color v�lido
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
	const int ancho = 600;
	//uso (2)
	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
	const int muestras_por_pixel  = 50;
	const int profundidad_maxima = 30;
	
	cout << "P3\n" << ancho << ' ' << alto << "\n255\n";

	lista_chocable mundo = escena_aleatoria();
	// mundo.agregar(make_shared<esfera>(punto3(0,-100.5,-1), 100, make_shared<lambertiano>(color(0.5,0.2,0.2))));
	
	// mundo.agregar(make_shared<esfera>(punto3(-1,0,-1), 0.5, make_shared<metalico>(color(0.8,0.8,0.8),0.0)));
	// mundo.agregar(make_shared<esfera>(punto3(-0.4,-0.3,-0.6), -0.15, make_shared<dialectrico>(1.5)));
	//  mundo.agregar(make_shared<esfera>(punto3(0,0,-1), 0.5, make_shared<dialectrico>(2.1)));
	// mundo.agregar(make_shared<esfera>(punto3(0.4,-0.3,-0.6), 0.15, make_shared<metalico>(color(1.0,1.0,0.0),0.2)));
	// mundo.agregar(make_shared<esfera>(punto3(1,0,-1), 0.5, make_shared<lambertiano>(color(0.9,0.9,0.3))));
	
	punto3 mirar_desde(13,2,3);
	punto3 mirar_hacia(0,0,0);
	vec3 vup(0,1,0);
	auto distancia_focal = 10; //(mirar_desde - mirar_hacia).longitud();
	auto apertura = 0.1;
	
	camara cam(mirar_desde,mirar_hacia,vup,20,relacion_de_aspecto,apertura,distancia_focal,0.0,1.0);
	
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
