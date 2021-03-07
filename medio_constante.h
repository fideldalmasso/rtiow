#ifndef MEDIO_CONSTANTE_H
#define MEDIO_CONSTANTE_H

#include "rtweekend.h"
#include "chocable.h"
#include "material.h"
#include "textura.h"

class medio_constante : public chocable{
public:
    medio_constante(shared_ptr<chocable> b, double d, shared_ptr<textura> t):
    borde(b),funcion_fase(make_shared<isotropico>(t)),densidad_negativa_invertida(-1/d){}

    medio_constante(shared_ptr<chocable> b, double d, color c):
    borde(b),funcion_fase(make_shared<isotropico>(c)),densidad_negativa_invertida(-1/d){}

    virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const override;

	virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override{
        return borde->caja_delimitadora(tiempo0,tiempo1,caja_saliente);
    }

public:
    shared_ptr<chocable> borde;
    shared_ptr<material> funcion_fase;
    double densidad_negativa_invertida;

};

bool medio_constante::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const{
    registro_choque registro1, registro2;

    if(!borde->choca(r,-infinito,infinito,registro1))
        return false;
    
    if(!borde->choca(r,registro1.t+0.0001,infinito,registro2))
        return false;

    if(registro1.t<t_min) registro1.t = t_min;
    if(registro2.t>t_max) registro2.t = t_max;

    if(registro1.t>=registro2.t)
        return false;
    
    if(registro1.t<0)
        registro1.t=0;

    const auto longitud_rayo = r.direccion().longitud();
    const auto distancia_dentro_borde = (registro2.t - registro1.t) * longitud_rayo;
    const auto distancia_de_choque = densidad_negativa_invertida * log(double_aleatorio());

    if(distancia_de_choque > distancia_dentro_borde)
        return false;
    
    registro.t = registro1.t + distancia_de_choque / longitud_rayo;
    registro.p = r.en(registro.t);

    registro.normal = vec3(1,0,0);
    registro.cara_frontal = true;
    registro.material_ptr = funcion_fase;
    
    return true;
}


#endif