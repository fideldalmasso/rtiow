//#include "rtweekend.h"
//#include "lista_chocable.h"
//#include "esfera.h"
//#include "color.h"
//#include "camara.h"
//#include "material.h"
//#include<iostream>
//#include <cstdio>
//#include <ctime>
//using namespace std;
//
//
//
//
//
//
//int main2() {
//	freopen("../out.ppm", "w", stdout);
//	//(1) rda = ancho / alto ->
//	//(2) ancho = rda * alto
//	//(3) alto = ancho / rda
//	
//	//uso(1)
//	const auto relacion_de_aspecto = 16.0 / 9.0;
//	const int ancho = 200;
//	//uso (2)
//	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
//	const int muestras_por_pixel  = 100;
//	const int profundidad_maxima = 100;
//	
//	cout << "P3\n" << ancho << ' ' << alto << "\n255\n";
//
//	lista_chocable mundo = escena_aleatoria();
//	// mundo.agregar(make_shared<esfera>(punto3(0,-100.5,-1), 100, make_shared<lambertiano>(color(0.5,0.2,0.2))));
//	
//	// mundo.agregar(make_shared<esfera>(punto3(-1,0,-1), 0.5, make_shared<metalico>(color(0.8,0.8,0.8),0.0)));
//	// mundo.agregar(make_shared<esfera>(punto3(-0.4,-0.3,-0.6), -0.15, make_shared<dialectrico>(1.5)));
//	//  mundo.agregar(make_shared<esfera>(punto3(0,0,-1), 0.5, make_shared<dialectrico>(2.1)));
//	// mundo.agregar(make_shared<esfera>(punto3(0.4,-0.3,-0.6), 0.15, make_shared<metalico>(color(1.0,1.0,0.0),0.2)));
//	// mundo.agregar(make_shared<esfera>(punto3(1,0,-1), 0.5, make_shared<lambertiano>(color(0.9,0.9,0.3))));
//	
//	punto3 mirar_desde(13,2,3);
//	punto3 mirar_hacia(0,0,0);
//	vec3 vup(0,1,0);
//	auto distancia_focal = 10; //(mirar_desde - mirar_hacia).longitud();
//	auto apertura = 0.1;
//	
//	camara cam(mirar_desde,mirar_hacia,vup,20,relacion_de_aspecto,apertura,distancia_focal);
//	
//	time_t inicio,fin;
//	time(&inicio);
//	for (int j = alto -1; j >= 0; --j) {
//		cerr << "\rScanlines remaining: " << j << ' ' << flush;
//		for (int i = 0; i < ancho; ++i) {
//			color pixel_color(0,0,0);
//			
//			
//			//el color de cada pixel es el promedio de los colores de ese pixel 
//			//junto con los colores de los pixeles que lo rodean.
//			//aqui solo se acumula la suma, pero en escribir_color, se divide
//			//por la cantidad de muestras
//			for(int s = 0; s < muestras_por_pixel; ++s){
//				auto u = (i + double_aleatorio()) / (ancho-1);
//				auto v = (j + double_aleatorio()) / (alto-1);
//				rayo r = cam.get_rayo(u,v);
//				pixel_color += color_de_rayo(r,mundo, profundidad_maxima);
//			}
//			escribir_color(cout,pixel_color, muestras_por_pixel);
//		}
//	}
//	time(&fin);
//	double tiempo_transcurrido = (double) fin - inicio;
//	fprintf(stderr,"\nTiempo transcurrido: %.1f",tiempo_transcurrido);
//	cerr << "\nDone.\n";
//}
