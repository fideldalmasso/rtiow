#include<iostream>
#include "vec3.h"
#include "color.h"
using namespace std;

int main() {
	const int ancho = 400;
	const int alto= 400;
	
	cout << "P3\n" << ancho << ' ' << alto << "\n255\n";
	
	for (int j = alto -1; j >= 0; --j) {
		cerr << "\rScanlines remaining: " << j << ' ' << flush;
		for (int i = 0; i < ancho; ++i) {
			
			//color es un alias de vec3!!!!!!
			color pixel_color(double(i)/(ancho-1),
							  double(j)/(alto-1),
							  0.25); 
			
			escribir_color(cout,pixel_color);
		}
	}
	cerr << "\nDone.\n";
}
