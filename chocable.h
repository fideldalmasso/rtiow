#ifndef CHOCABLE_H
#define CHOCABLE_H

#include "rtweekend.h"
#include "aabb.h"

class material; //no se para que se usa, pero supuestamente el puntero que se usa mas abajo apunta a una clase.

struct registro_choque{
	punto3 p;
	vec3 normal;
	shared_ptr<material> material_ptr;
	double t;
	double u;
	double v;
	bool cara_frontal;
	
	inline void set_cara_y_normal(const rayo& r, const vec3& normal_saliente){

		// es true cuando el rayo y la normal estan del mismo lado
		cara_frontal = producto_punto(r.direccion(),normal_saliente) < 0; 
		// si estan del mismo lado, la normal es igual a la saliente, sino es la contraria
		normal = cara_frontal ? normal_saliente : -normal_saliente;
	}
	
};


class chocable {
public:
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const = 0;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const = 0;
};

class trasladar : public chocable{
public:
	trasladar(shared_ptr<chocable> objeto, const vec3& desplazamiento):
	ptr(objeto),offset(desplazamiento){}

	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override;
public:
	shared_ptr<chocable> ptr;
	vec3 offset;

};

bool trasladar::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{
	
	rayo rayo_desplazado(r.origen()-offset,r.direccion(),r.tiempo());
	
	if(!ptr->choca(rayo_desplazado,t_min,t_max,registro))
		return false;

	registro.p+=offset;
	registro.set_cara_y_normal(rayo_desplazado,registro.normal);
	return true;

}

bool trasladar::caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const{

	if(!ptr->caja_delimitadora(tiempo0,tiempo1,caja_saliente))
		return false;
	
	caja_saliente = aabb(caja_saliente.min()+offset, caja_saliente.max()+offset);
	
	return true;
}

class rotar_y : public chocable{
public:
	rotar_y(shared_ptr<chocable> objeto, double angulo);
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override{
		caja_saliente = caja_delim;
		return tiene_caja;
	}

	inline punto3 aux_rotar_y(const punto3& original) const{
		return punto3(cos_tita*original.x()+sen_tita*original.z(), original.y(), -sen_tita*original.x()+cos_tita*original.z());
	}

	inline punto3 aux_rotar_y2(const punto3& original) const{
		return punto3(cos_tita*original.x()-sen_tita*original.z(), original.y(), sen_tita*original.x()+cos_tita*original.z());
	}

public:
	shared_ptr<chocable> ptr;
	double sen_tita;
	double cos_tita;
	bool tiene_caja;
	aabb caja_delim;

};

rotar_y::rotar_y(shared_ptr<chocable> objeto, double angulo) : ptr(objeto){
	auto radianes = grados_a_radianes(angulo);
	sen_tita = sin(radianes);
	cos_tita = cos(radianes);

	tiene_caja = ptr->caja_delimitadora(0,1,caja_delim);
	punto3 min(infinito,infinito,infinito);
	punto3 max(-infinito,-infinito,-infinito);
	  
	for(int i=0; i<2; i++){
		for(int j=0; j<2; j++){
			for(int k=0; k<2; k++){
				auto x = i*caja_delim.max().x() + (1-i)*caja_delim.min().x();
				auto y = j*caja_delim.max().y() + (1-j)*caja_delim.min().y();
				auto z = k*caja_delim.max().z() + (1-k)*caja_delim.min().z();

				// auto nuevo_x = cos_tita*x+sen_tita*z;
				// auto nuevo_z = -sen_tita*x+cos_tita*z;

				// vec3 tester(nuevo_x,y,nuevo_z);
				vec3 tester = aux_rotar_y(punto3(x,y,z));

				for(int c = 0; c<3; c++){
					min[c] = fmin(min[c],tester[c]);
					max[c] = fmax(max[c],tester[c]);
				}
				

			}
		}
	}

	caja_delim = aabb(min,max);

}

bool rotar_y::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const {

	punto3 origen = aux_rotar_y2(r.origen());
	punto3 direccion = aux_rotar_y2(r.direccion());

	rayo rayo_rotado(origen,direccion,r.tiempo());

	if(!ptr->choca(rayo_rotado,t_min,t_max,registro))
		return false;

	auto p = aux_rotar_y(registro.p);
	auto normal = aux_rotar_y(registro.normal);

	registro.p = p;
	registro.set_cara_y_normal(rayo_rotado,normal);
	
	return true;
}



#endif