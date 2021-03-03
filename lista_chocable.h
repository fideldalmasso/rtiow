#ifndef LISTA_CHOCABLE_H
#define LISTA_CHOCABLE_H

#include "aabb.h"
#include "chocable.h"

#include <memory>
#include <vector>


using std::shared_ptr;
using std::make_shared;


class lista_chocable: public chocable {
public:
	lista_chocable(){}
	lista_chocable(shared_ptr<chocable> objeto) {agregar(objeto);}
	
	void borrar() {objetos.clear();}
	void agregar(shared_ptr<chocable> objeto) { objetos.push_back(objeto);}
	
	virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override;
	
public:
	std::vector<shared_ptr<chocable>> objetos;
};

bool lista_chocable::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const {
	registro_choque registro_temp;
	
	bool choca_algo = false;
	
	auto mas_cercano_hasta_ahora = t_max;
	
	for(const auto& objeto : objetos){
		if(objeto->choca(r, t_min, mas_cercano_hasta_ahora, registro_temp)) {
			choca_algo = true;
			mas_cercano_hasta_ahora = registro_temp.t;
			registro = registro_temp;
		}
	}
	return choca_algo;
	
	
}


bool lista_chocable::caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const{
	if(objetos.empty())
		return false;
	
	aabb caja_temp;
	bool es_la_primer_caja = true;
	
	for(const auto& objeto : objetos){
		if(!objeto->caja_delimitadora(tiempo0,tiempo1,caja_temp)) return false;
		
		caja_saliente = es_la_primer_caja  ? caja_temp : caja_agrupadora(caja_saliente,caja_temp);
		es_la_primer_caja = false;
	}
	return true;
	
}




#endif

