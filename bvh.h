#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "rtweekend.h"
#include "chocable.h"
#include "lista_chocable.h"

class nodo_bvh : public chocable {
public:
	nodo_bvh();
	
	nodo_bvh(const lista_chocable& lista, double tiempo0, double tiempo1): 
		nodo_bvh(lista.objetos,0,lista.objetos.size(), tiempo0, tiempo1) {}
	
	nodo_bvh(const std::vector<shared_ptr<chocable>>& objetos_src,
			 size_t inicio, size_t fin, double tiempo0, double tiempo1);
	
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;
	
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override;
	
public:
	shared_ptr<chocable> izquierda;
	shared_ptr<chocable> derecha;
	aabb caja;
	
};

bool nodo_bvh::caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const{
	caja_saliente = caja;
	return true;
}

bool nodo_bvh::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{
	if(!caja.choca(r,t_min,t_max))
		return false;
	
	bool choca_izquierda = izquierda->choca(r,t_min,t_max,registro);
	bool choca_derecha = derecha->choca(r,t_min,choca_izquierda? registro.t: t_max,registro);
	
	//ese condicional es necesario porque necesito saber cual nodo choca primero, si es que choca ambos
	
	return choca_izquierda || choca_derecha;
	
}

inline bool comparar_caja(const shared_ptr<chocable> a, const shared_ptr<chocable> b, int eje){
	aabb caja_a;
	aabb caja_b;
	
	if(!a->caja_delimitadora(0,0,caja_a) || !b->caja_delimitadora(0,0,caja_b))
		std::cerr << "No hay caja delimitadora en el constructor de nodo_bvh\n";
	
	return caja_a.min().e[eje] < caja_b.min().e[eje];
}
	
bool comparar_caja_x(const shared_ptr<chocable> a, const shared_ptr<chocable> b){return comparar_caja(a,b,0);}
bool comparar_caja_y(const shared_ptr<chocable> a, const shared_ptr<chocable> b){return comparar_caja(a,b,1);}
bool comparar_caja_z(const shared_ptr<chocable> a, const shared_ptr<chocable> b){return comparar_caja(a,b,2);}

nodo_bvh::nodo_bvh(const std::vector<shared_ptr<chocable>>& objetos_src,
		 size_t inicio, size_t fin, double tiempo0, double tiempo1){
	
	auto objetos = objetos_src;
	int eje = int_aleatorio(0,2);
	
	auto comparador = (eje == 0) ? comparar_caja_x
					: (eje == 1) ? comparar_caja_y
								 : comparar_caja_z;
	
	size_t intervalo = fin - inicio;
	
	if(intervalo ==1){
		izquierda = derecha = objetos[inicio];
	}
	else if(intervalo == 2){
		if(comparador(objetos[inicio],objetos[inicio+1])){
			izquierda = objetos[inicio];
			derecha = objetos[inicio+1];
		}
		else{
			izquierda = objetos[inicio+1];
			derecha = objetos[inicio];
		}
	}
	else{
		std::sort(objetos.begin() + inicio, objetos.begin() + fin, comparador);
		
		auto mitad = inicio + intervalo/2;
		izquierda = make_shared<nodo_bvh>(objetos,inicio,mitad,tiempo0,tiempo1);
		derecha = make_shared<nodo_bvh>(objetos,mitad,fin,tiempo0,tiempo1);
		
	}
	
	aabb caja_izquierda, caja_derecha;
	
	if(!izquierda->caja_delimitadora(tiempo0,tiempo1,caja_izquierda) || !derecha->caja_delimitadora(tiempo0,tiempo1,caja_derecha))
		std::cerr << "No hay caja delimitadora en el constructor de nodo_bvh\n";
	
	caja = caja_agrupadora(caja_izquierda,caja_derecha);
}




#endif
