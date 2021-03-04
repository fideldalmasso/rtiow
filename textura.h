#ifndef TEXTURA_H
#define TEXTURA_H

#include "rtweekend.h"
#include "perlin.h"

class textura{
    public:
        virtual color valor(double u, double v, const punto3& p) const = 0;
};

class color_solido : public textura{
    public:
        color_solido(){}
        
        color_solido(color c) : valor_de_color(c){}

        color_solido(double rojo, double verde, double azul) : color_solido(color(rojo,verde,azul)){}

        virtual color valor(double u, double v, const punto3& p ) const override{
            return valor_de_color;
        }

    private:
        color valor_de_color;
};


class textura_damas : public textura{
    public:
        textura_damas(){}
        
        textura_damas(shared_ptr<textura> _par, shared_ptr<textura> _impar):
        par(_par),impar(_impar){}

        textura_damas(color c1,color c2):
        par(make_shared<color_solido>(c1)),impar(make_shared<color_solido>(c2)){}


        virtual color valor(double u, double v, const punto3& p) const override{
            auto senos = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
            if(senos < 0)
                return impar->valor(u,v,p);
            else
                return par->valor(u,v,p);
        }

    public:
        shared_ptr<textura> par;
        shared_ptr<textura> impar;    
};


class textura_ruido : public textura{
    public:
        textura_ruido(){}

        virtual color valor(double u, double v, const punto3& p) const override{
            return color(1,1,1)*ruido.ruido(p);
        }
    public:
        perlin ruido;
};

#endif