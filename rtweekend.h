#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinito = std::numeric_limits<double>::infinity();
const double pi = 3.141592653897932385;

inline double grados_a_radianes(double grados){
	return grados * pi / 180;
}
	
//https://thebookofshaders.com/glossary/?search=clamp
inline double clamp(double x, double min, double max){
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
	
inline double double_aleatorio(){ //numero entre 0 y 1
	// return rand() / (RAND_MAX + 1.0);
	static std::uniform_real_distribution<double> distribution(0.0,1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double double_aleatorio(double min, double max){
	return min + (max - min) * double_aleatorio();
}	
	
inline int int_aleatorio(int min, int max){
	return static_cast<int>(double_aleatorio(min,max+1));
}
	
	
//dejar estos include aca!!! sino se rompe todo mal
#include "vec3.h"
#include "rayo.h"
#include "chocable.h"
	

#endif
