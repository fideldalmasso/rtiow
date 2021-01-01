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


lista_chocable escena_aleatoria1(){
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

lista_chocable escena_aleatoria2(){
	lista_chocable mundo;
	//auto material_esfera_voladora = make_shared<metalico>(color(1.0,1.0,1.0),0.0);
	//mundo.agregar(make_shared<esfera>(punto3(0.0,7,-10),7,material_esfera_voladora));

	auto material_suelo = make_shared<lambertiano>(color(0.7,0.7,0.7));
	mundo.agregar(make_shared<esfera>(punto3(0,-1000,0),1000,material_suelo));
	
	auto material1 = make_shared<dialectrico>(1.4);
	punto3 centro_grande(-1,1.5,0);
	mundo.agregar(make_shared<esfera>(centro_grande,1.5,material1));
	auto material2 = make_shared<metalico>(color(1.0,1.0,1.0),0.08);
	punto3 centro_grande2(1.5,1.7,-3);
	mundo.agregar(make_shared<esfera>(centro_grande2,2.0,material2));
	color azul;
		for(int i =0; i<50; i++){
			auto elegir_material = double_aleatorio();
			auto radio  = double_aleatorio(0.05,0.2);
			punto3 centro(vector_en_esfera_unitaria_aleatorio()*1.3+centro_grande);
			azul = color(double_aleatorio(0.0,0.2),double_aleatorio(0.0,0.2),double_aleatorio(0.5,0.95));
			shared_ptr<material> material_esfera;

				if(elegir_material < 0.4){
					//difuso
					auto albedo = azul;
					material_esfera = make_shared<lambertiano>(albedo);
					
				}
				else if(elegir_material < 0.7){
					//metal
					auto albedo = azul;
					auto aspereza = double_aleatorio(0,0.5);
					material_esfera = make_shared<metalico>(albedo,aspereza);
				}
				else{
					//vidrio
					material_esfera = make_shared<dialectrico>(1.5);
					
				}
				
				mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
			
		}

		color rojo;
		for(int x = -20; x<20; x++){
				//double r = clamp((double)(x+50)/100,0.0,1.0);
				//cout<< r << endl;
			for(int z = -40; z<15; z++){
				// if(z<-10 && x%4==0){
				// 	continue;
				// }
				rojo = color(double_aleatorio(0.7,0.95),double_aleatorio(0.0,0.2),double_aleatorio(0.0,0.2));
				//double g = clamp(1.0- (z+30)*2.5/100,0.0,1.0);
				//double b = double_aleatorio(0.0,1.0);
				//rojo = color(r,g,b);
				auto elegir_material = double_aleatorio();
				auto radio  = double_aleatorio(0.1,0.4);
				double x1 = x + 0.9*double_aleatorio();
				double z1 =  z + 0.9*double_aleatorio();
				punto3 centro(x1,sqrt(1000000 -x1*x1- z1*z1) -1000 + radio,z1);
				
				shared_ptr<material> material_esfera;
				if((centro_grande - centro).longitud() < 2 || (centro_grande2 - centro).longitud() < 2.1 || double_aleatorio() < 0.4)
					continue;
				if(elegir_material < 0.8){
					//difuso
					auto albedo = rojo;
					material_esfera = make_shared<lambertiano>(albedo);
					
				}
				else if(elegir_material < 0.95){
					//metal
					auto albedo = rojo;
					//auto aspereza = double_aleatorio(0,0.2);
					material_esfera = make_shared<metalico>(albedo,0.0);
				}
				else{
					//vidrio
					material_esfera = make_shared<dialectrico>(1.5);
					
				}
				
				mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
				
			}
		}
		
		
	
	
	
		
		return mundo;
	
}
	

int main(){
	
	const auto relacion_de_aspecto = 16.0/9.0;
	const int ancho = 1920;
	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
	const int muestras_por_pixel  = 200;
	const int profundidad_maxima = 50;

	lista_chocable mundo = escena_aleatoria2();
    int cantidad_hilos = thread::hardware_concurrency();
    cout<< "Hilos detectados: " << cantidad_hilos << "\n";
	
	punto3 mirar_desde(0,3,15);
	punto3 mirar_hacia(0,1.5,0);
	vec3 vup(0,1,0);
	auto distancia_focal = (mirar_desde - mirar_hacia).longitud(); //10;
	auto apertura = 0.1;
	
	camara cam(mirar_desde,mirar_hacia,vup,20,relacion_de_aspecto,apertura,distancia_focal);
	
	time_t inicio,fin;
	time(&inicio);
	escena e(ancho,alto,muestras_por_pixel,profundidad_maxima,mundo,cam,16,cantidad_hilos);

	e.ejecutar();
	ofstream archivo;
	archivo.open("../out.ppm");
	archivo << "P3\n" << ancho << ' ' << alto << "\n255\n";
	
	int total = alto * ancho ;
	for(int i = 0; i<total; i++){
		
		double r = e.datos[i].x();
		double g = e.datos[i].y();
		double b = e.datos[i].z();
		escribir_color(archivo,
					   color(r,g,b),
					   muestras_por_pixel);
	}
	archivo.close();
	time(&fin);
	double tiempo_transcurrido = (double) fin - inicio;
	printf("\nTiempo transcurrido: %.1f",tiempo_transcurrido);
	getchar();
	return 0;
}
