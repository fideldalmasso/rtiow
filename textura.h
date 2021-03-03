#ifndef TEXTURA_H
#define TEXTURA_H

#include "rtweekend.h"

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


#endif