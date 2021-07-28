#ifndef ESCENA_H
#define ESCENA_H

#include "rtweekend.h"
#include "lista_chocable.h"
#include "esfera.h"
#include "color.h"
#include "camara.h"
#include "material.h"
#include <iostream>
#include <cstdio>

class escena{
public:
	
	escena(int _ancho, int _alto, int _muestras_por_pixel, int _profundidad_maxima, const lista_chocable & _mundo, const camara & _cam, int _cantidad_hilos){
		ancho= _ancho;
		alto= _alto;
		muestras_por_pixel= _muestras_por_pixel;
		profundidad_maxima= _profundidad_maxima;
		mundo = _mundo;
		cam= _cam;
		cantidad_hilos = _cantidad_hilos;
		total_pixeles = ancho * alto;
		}
	
	
public:
	int ancho, alto, muestras_por_pixel, profundidad_maxima, total_pixeles, cantidad_hilos;
	lista_chocable mundo;
	camara cam;
	
};

#endif
