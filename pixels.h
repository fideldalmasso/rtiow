#ifndef PIXELS_H
#define PIXELS_H
#include "rtweekend.h"

#include <vector>
using namespace std;
class pixels {
public:
	pixels(){}
	pixels(const int& an, const int& al){
		ancho = an;
		alto = al;
		datos = vector<double>(ancho * alto * 3);
	}
		
	void acumular(int pos, const color &valor){
		pos*=3;
		datos[pos] = valor.x();
		datos[pos+1] = valor.y();
		datos[pos+2] = valor.z();
	}
	inline void acumular(int x,int y, const color &valor){
		int pos = y*ancho + x * 3;
		datos[pos] = valor.x();
		datos[pos+1] = valor.y();
		datos[pos+2] = valor.z();
	}
	
public:
	int ancho, alto;
	vector<double> datos;
};


#endif
