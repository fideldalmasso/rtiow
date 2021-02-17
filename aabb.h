#ifndef AABB_H
#define AABB_H
#include "rtweekend.h"

class aabb{
public:
	aabb(){}
	aabb(const punto3& a, const punto3& b){minimo=a;maximo=b;}
	
	punto3 min() const {return minimo;}
	punto3 max() const {return maximo;}
	
	inline bool choca(const rayo& r, double t_min, double t_max) const {
		
		// sabemos que la recta es y = A + t*B
		// y que los puntos min y max definen los planos de interseccion de la bounding box
		// luego, para cada x0 osea x,y,z, se cumple que x0 = A + t0*B
		// por lo tanto t0 = (x0 - A) / B
		// recordemos que B es la pendiente del rayo, y A es la ordenada al origen
		
		
		for(int a=0; a<3;a++){
			auto inversa = 1.0f / r.direccion()[a];         // invesa = 1/B
			auto t0 = (min()[a] - r.origen()[a]) * inversa; // t0 = (x0 - A) / B   tiempo en el que se interseca el primer plano
			auto t1 = (max()[a] - r.origen()[a]) * inversa; // t1 = (x1 - A) / B   tiempo en el que se interseca el segundo plano
			
			if(inversa < 0.0f) //la pendiente es negativa
				std::swap(t0,t1);
			
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_min ? t1 : t_max;
			
			if(t_max <= t_min)
				return false;
		}
		
		return true;
	}
	
	static const aabb caja_englobadora(aabb caja0, aabb caja1){
		punto3 punto_mas_chico(fmin(caja0.min().x(), caja1.min().x()),
					 fmin(caja0.min().y(), caja1.min().y()),
					 fmin(caja0.min().z(), caja1.min().z()));
		
		punto3 punto_mas_grande(fmin(caja0.max().x(), caja1.max().x()),
					 fmin(caja0.max().y(), caja1.max().y()),
					 fmin(caja0.max().z(), caja1.max().z()));
		
		return aabb(punto_mas_chico,punto_mas_grande);
	}
	
	punto3 minimo;
	punto3 maximo;	
};



#endif
