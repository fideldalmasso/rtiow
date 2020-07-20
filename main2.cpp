#include "rtweekend.h"
#include "lista_chocable.h"
//#include "esfera.h"
#include "color.h"
#include "camara.h"
//#include "material.h"
#include "escena.h"
#include<iostream>
#include <cstdio>
#include <ctime>
#include <thread>
#include <fstream>
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
				
			}
			else if(elegir_material < 0.95){
				//metal
				auto albedo = color::aleatorio(0.5,1);
				auto aspereza = double_aleatorio(0,0.5);
				material_esfera = make_shared<metalico>(albedo,aspereza);
			}
			else{
				//vidrio
				material_esfera = make_shared<dialectrico>(1.5);
				
			}
			
			mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
			
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


int main(){
	
	const auto relacion_de_aspecto = 16.0/9.0;
	const int ancho = 300;
	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
	const int muestras_por_pixel  = 500;
	const int profundidad_maxima = 50;

	lista_chocable mundo = escena_aleatoria();
    int cantidad_hilos = thread::hardware_concurrency();
    cout<< "Hilos detectados: " << cantidad_hilos << "\n";
	
	punto3 mirar_desde(13,2,3);
	punto3 mirar_hacia(0,0,0);
	vec3 vup(0,1,0);
	auto distancia_focal = 10; //(mirar_desde - mirar_hacia).longitud();
	auto apertura = 0.1;
	
	camara cam(mirar_desde,mirar_hacia,vup,20,relacion_de_aspecto,apertura,distancia_focal);
	
	time_t inicio,fin;
	time(&inicio);
	escena e(ancho,alto,muestras_por_pixel,profundidad_maxima,mundo,cam,8,cantidad_hilos);

	e.ejecutar();
	ofstream archivo;
	archivo.open("../out.ppm");
	//freopen("../out.ppm", "w", stdout);
	archivo << "P3\n" << ancho << ' ' << alto << "\n255\n";
	
	int total = alto * ancho * 3;
	for(int i = 0; i<total; i+=3){
		
		//cout << e.pantalla.datos[i] << " " << e.pantalla.datos[i+1] << " " << e.pantalla.datos[i+2] << "\n";
		double r = e.pantalla.datos[i];
		double g = e.pantalla.datos[i+1];
		double b = e.pantalla.datos[i+2];
		escribir_color(archivo,
					   color(r,g,b),
					   muestras_por_pixel);
	}
	archivo.close();
	time(&fin);
	double tiempo_transcurrido = (double) fin - inicio;
	printf("\nTiempo transcurrido: %.1f",tiempo_transcurrido);
	
	return 0;
}
