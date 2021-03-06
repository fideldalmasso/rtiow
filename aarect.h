#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"
#include "chocable.h"

class rectangulo_xy : public chocable{
    public:
        rectangulo_xy(){}

        rectangulo_xy(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat):
        x0(_x0),x1(_x1),y0(_y0),y1(_y1),k(_k),mp(mat){}

        virtual bool choca(const rayo& r, double t_min, double t_max, registro_choque& rec) const override;

        virtual bool caja_delimitadora(double tiempo0, double tiempo1, aabb& caja_saliente) const override {
            caja_saliente = aabb(punto3(x0,y0,k-0.0001),punto3(x1,y1,k+0.0001));
            return true;
        }

    public:
        double x0,x1,y0,y1,k;
        shared_ptr<material> mp;
        
};

bool rectangulo_xy::choca(const rayo& r, double t_min, double t_max, registro_choque& registro) const {

    /*
    Un rayo puede escribirse como P(t) = A + t*b donde A es un punto y b es la direccion
    
    Resolviendo solo para la componente en z queda
    z = Az + t*bz

    la interseccion con nuestro plano es para z = k entonces
    k = Az + t*bz
    t = (k - Az)/bz

    usamos este t para hallar la solucion para las componentes x e y
    x = Ax + t*bx
    y = Ay + t*by

    el rayo toca el rectangulo si los x e y hallados pertenecen al rectangulo
    */

    auto t = (k-r.origen().z()) / r.direccion().z();
    if(t < t_min || t > t_max)
        return false;
    auto x = r.origen().x() + t*r.direccion().x();
    auto y = r.origen().y() + t*r.direccion().y();

    if(x<x0 || x>x1 || y<y0 || y>y1)
        return false;

    //normaliza las coordenadas para la textura
    registro.u = (x-x0)/(x1-x0);
    registro.v = (y-y0)/(y1-y0);
    registro.t = t;

    auto normal_saliente = vec3(0,0,1);
    registro.set_cara_y_normal(r,normal_saliente);
    registro.material_ptr = mp;
    registro.p = r.en(t);
    return true;
}


#endif