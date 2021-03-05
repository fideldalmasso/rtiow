#ifndef TEXTURA_H
#define TEXTURA_H

#include "rtweekend.h"
#include "perlin.h"
#include "rtw_stb_image.h"

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
        textura_ruido(double esc) : escala(esc) {}

        virtual color valor(double u, double v, const punto3& p) const override{
            //el mas basico
            //return color(1,1,1)*ruido.ruido(escala*p);

            //usar vectores y no doubles. es necesario mapear [-1,1] a [0,1]
            //return color(1,1,1)*0.5*(1.0+ruido.ruido(escala*p));

            //usar turbulencia
            //return color(1,1,1)*ruido.turbulencia(escala*p);

            //efecto marmol
            return color(1,1,1)*0.5*(1 + sin(escala*p.z() + 10*ruido.turbulencia(p)));
        }
    public:
        perlin ruido;
        double escala;
};


class textura_imagen : public textura{
    public: 
        const static int bytes_por_pixel = 3;

        textura_imagen():
        datos(nullptr),ancho(0),alto(0),bytes_por_linea(0){}

        textura_imagen(const char* nombre_archivo){
            auto componentes_por_pixel = bytes_por_pixel;
            
            datos = stbi_load(nombre_archivo,&ancho,&alto,&componentes_por_pixel,componentes_por_pixel);
            
            if(!datos){
                std::cerr << "ERROR: No se pudo cargar la imagen de la textura '" << nombre_archivo << "'\n";
                ancho = alto = 0;
            }
            bytes_por_linea = bytes_por_pixel*ancho;

            
        }

        ~textura_imagen(){
            delete datos;
        }

        virtual color valor(double u, double v, const vec3& p) const override{
            if(datos==nullptr)
                return color(0,1,1);

            u = clamp(u,0.0,1.0);
            v = 1.0 - clamp(v,0.0, 1.0);

            auto i = static_cast<int>(u*ancho);
            auto j = static_cast<int>(v*alto);

            if(i>=ancho) i = ancho-1;
            if(j>=alto) j = alto-1;

            const auto escala_de_color = 1.0 / 255.0;
            auto pixel = datos + j*bytes_por_linea + i*bytes_por_pixel;

            return color(escala_de_color*pixel[0],escala_de_color*pixel[1],escala_de_color*pixel[2]);
        }

    private:
        unsigned char* datos;
        int ancho, alto;
        int bytes_por_linea;
};



#endif