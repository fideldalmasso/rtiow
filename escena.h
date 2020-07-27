#ifndef ESCENA_H
#define ESCENA_H

#include "pixels.h"
#include "rtweekend.h"
#include "lista_chocable.h"
#include "esfera.h"
#include "color.h"
#include "camara.h"
#include "material.h"
#include<iostream>
#include <cstdio>
#include <ctime>
#include <thread>
#include <mutex>

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

class escena{
public:
	
	escena(int ancho1, int alto1, int muestras, int prof, const lista_chocable &mundo1, const camara& cam1, int tamTarea, int canthilos){
		ancho= ancho1;
		alto= alto1;
		muestras_por_pixel= muestras;
		profundidad_maxima= prof;
		mundo= mundo1;
		cam= cam1;
		N= tamTarea;
		cantidad_hilos = canthilos;
		datos = new color[ancho*alto];
		total_pixeles = ancho * alto;
		total_tareas = total_pixeles%N==0?total_pixeles/N:total_pixeles/N+1;
		hilos = vector<thread>(cantidad_hilos);
		tarea_siguiente=0;
		}
	
	void imprimir(){
		int progreso = tarea_siguiente * 100 / total_tareas ;
		printf("\rProgreso: %i%%",progreso);
	}
		
	void ejecutar(){
		for(thread &h : hilos)
			h = thread(&escena::tarea,this);
		for(thread &h : hilos)
			h.join();
	}
	
	inline int buscar_tarea(){
		unique_lock<mutex> guard(candado);
		int x = -1;
			if(tarea_siguiente == total_tareas)
				return -1;
			else {
		x = tarea_siguiente;
				tarea_siguiente++;
			}
		guard.unlock();
		return x;
	}
	
	 void tarea(){
		int t = 0;
		 while(true){
			
			 t = buscar_tarea();
			if(t == -1) //tarea no encontrada
				return;
			else{
				
				
				int inicio = t * N;
				int fin = inicio + N;
				if(t>=total_tareas-1)
					fin = total_pixeles;
				
				int j,i;
				
				for(;inicio<fin;inicio++){
					j = alto -1 -  (inicio / ancho);
					i = inicio % ancho;
					
					color pixel_color(0,0,0);

					for(int s = 0; s < muestras_por_pixel; ++s){
						auto u = (i + double_aleatorio()) / (ancho-1);
						auto v = (j + double_aleatorio()) / (alto-1);
						rayo r = cam.get_rayo(u,v);
						pixel_color += color_de_rayo(r,mundo, profundidad_maxima);
					}
					datos[inicio] = pixel_color;
					
				}
				imprimir();

			}
		 }
		 
		 
		 
		 
	 }

			

	
public:
	color *  datos;
	pixels pantalla;
			 
private:
	int N, ancho, alto, muestras_por_pixel, profundidad_maxima, total_pixeles, total_tareas, cantidad_hilos;
	lista_chocable mundo;
	camara cam;
	//vector<bool> completadas;
	vector<thread> hilos;
	mutex candado;
	int tarea_siguiente;
	
};

#endif
