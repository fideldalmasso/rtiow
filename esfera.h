#ifndef SPHERE_H
#define SPHERE_H

class esfera: public chocable {
public:
	sphere() {}
	sphere(punto3 cen, double r) : centro(cen), radio(r) {};
	
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& rec) const;
	
public:
	point3 centro;
	double radio;
};


bool esfera:choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{

	vec3 co = r.origen() - centro; 
	vec3 u = r.direccion();
	auto a = u.longitud_cuadrada();	
	auto medio_b = producto_punto(co,u);
	auto c = co.longitud_cuadrada() - radio*radio;
	auto discriminante = medio_b*medio_b - a*c;
	
	if(discriminante>0){ //el rayo toca la esfera
		auto raiz = sqrt(discriminante);
		auto temp = (-medio_b - raiz) / a; //formula resolvente
		
		if(temp < t_max && temp > t_min){ //pruebo la formula con -
			registro.t = temp;
			registro.p = r.en(registro.t);
			registro.normal = (registro.p - centro) / radio;
			return true;
		}
		temp = (-medio_b + raiz) / a;
		
		if(temp < t_max && temp > t_min){ //pruebo la formula con +
			registro.t = temp;
			registro.p = r.en(registro.t);
			registro.normal = (registro.p - centro) / radio;
			return true;
		}
	}
	return false; //el rayo no toca la esfera
	
}




#endif

