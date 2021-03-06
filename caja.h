#ifndef CAJA_H
#define CAJA_H
#include <aarect.h>
#include <lista_chocable.h>

class caja : public chocable{
public:
    caja(){}

    caja(const punto3& p0, const punto3& p1, shared_ptr<material> ptr);

    virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;
	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override{
        caja_saliente = aabb(min_caja,max_caja);
        return true;
    }

public:
    punto3 min_caja;
    punto3 max_caja;
    lista_chocable lados;
};

caja::caja(const punto3& p0, const punto3& p1, shared_ptr<material> ptr){
    min_caja = p0;
    max_caja = p1;

    lados.agregar(make_shared<rectangulo_xy>(p0.x(),p1.x(),p0.y(),p1.y(),p1.z(),ptr));
    lados.agregar(make_shared<rectangulo_xy>(p0.x(),p1.x(),p0.y(),p1.y(),p0.z(),ptr));
    
    lados.agregar(make_shared<rectangulo_xz>(p0.x(),p1.x(),p0.z(),p1.z(),p1.y(),ptr));
    lados.agregar(make_shared<rectangulo_xz>(p0.x(),p1.x(),p0.z(),p1.z(),p0.y(),ptr));
    
    lados.agregar(make_shared<rectangulo_yz>(p0.y(),p1.y(),p0.z(),p1.z(),p1.x(),ptr));
    lados.agregar(make_shared<rectangulo_yz>(p0.y(),p1.y(),p0.z(),p1.z(),p0.x(),ptr));
}

bool caja::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{
    return lados.choca(r,t_min,t_max,registro);
}



#endif