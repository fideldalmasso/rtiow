#ifndef PERLIN_H
#define PERLIN_H
#include "rtweekend.h"


class perlin{
    public:
        perlin(){
            vectores_aleatorios = new vec3[cant_puntos];
            for(int i = 0; i< cant_puntos; ++i){
                vectores_aleatorios[i] = vector_unitario(vec3::aleatorio(-1,1));
            }

            perm_x = perlin_generar_perm();
            perm_y = perlin_generar_perm();
            perm_z = perlin_generar_perm();

        }

        ~perlin(){
            delete[] vectores_aleatorios;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        double ruido(const vec3& p) const{

            //u,v,w son la parte no entera de x,y,z
            //osea, son la distancia a la grilla de numeros enteros
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            //i,j,k son la parte entera de x,y,z
            auto i = static_cast<int>(floor(p.x()));
            auto j = static_cast<int>(floor(p.y()));
            auto k = static_cast<int>(floor(p.z()));

            //calcular los 8 puntos del cubo que contiene a p
            //& == AND
            //^ == XOR
            //x&255 == mod(x,256)
            vec3 c[2][2][2];
            for(int di=0; di<2; di++){
                for(int dj=0; dj<2; dj++){
                    for(int dk=0; dk<2; dk++){
                        c[di][dj][dk] = vectores_aleatorios[perm_x[(i+di)&255] ^ perm_y[(j+dj)&255] ^ perm_z[(k+dk)&255]];
                    }
                }
            }
            
            return interpolacion_perlin(c,u,v,w);
        }

        double turbulencia(const punto3& p, int profundidad=7) const {
            //consiste en la suma de varias llamadas a ruido
            auto accum = 0.0;
            auto temp_p = p;
            auto peso = 1.0;

            for(int i=0; i< profundidad; i++){
                accum+= peso*ruido(temp_p);
                peso *= 0.5;
                temp_p *= 2; //duplica el tamanio del punto
            }
            return fabs(accum);
        }

    private:
        static const int cant_puntos = 256;
        vec3* vectores_aleatorios; //arreglo con 256 vectores pseudo-aleatorios (doubles)
        int* perm_x;             //arreglo con numeros del 0 a 255, desordenados
        int* perm_y;             //lo mismo aca
        int* perm_z;             //lo mismo aca

        static int* perlin_generar_perm(){
            auto p  = new int[cant_puntos];
            
            for(int i = 0; i< perlin::cant_puntos; i++)
                p[i] = i;
            
            permutar(p,cant_puntos);
            return p; 
        }

        static void permutar(int * p, int n){
            for (int i = n-1; i>0; i--){
                int objetivo = int_aleatorio(0,i);
                int temp = p[i];
                p[i] = p[objetivo];
                p[objetivo] = temp;
            }
        }

        static double interpolacion_trilineal(double c[2][2][2], double u, double v, double w){
            auto accum = 0.0;
            
            for(int i=0; i<2; i++){
                for(int j=0; j<2; j++){
                    for(int k=0; k<2; k++){
                        accum += (i*u + (1-i)*(1-u))*
                                 (j*v + (1-j)*(1-v))*   
                                 (k*w + (1-k)*(1-w))*
                                 c[i][j][k];
                    }
                }
            }
            return accum;
        }

        static double interpolacion_perlin(vec3 c[2][2][2], double u, double v, double w){
            //la funcion f(x) = 3x^2 - 2x^3 entre [0,1] es una ease curve y se conoce como SMOOTHSTEP
            //lo que hace es exagerar la proximidad a los extremos de un valor entre [0,1]
            //por ejemplo: smoothstep(0.8) -> 0.896
            //             smoothstep(0.5) -> 0.5
            auto uu = u*u*(3-2*u);
            auto vv = v*v*(3-2*v);
            auto ww = w*w*(3-2*w);
            auto accum = 0.0;
            
            for(int i=0; i<2; i++){
                for(int j=0; j<2; j++){
                    for(int k=0; k<2; k++){
                        vec3 peso_v(u-i,v-j,w-k);

                        accum += (i*uu + (1-i)*(1-uu))*
                                 (j*vv + (1-j)*(1-vv))*   
                                 (k*ww + (1-k)*(1-ww))*
                                 producto_punto(c[i][j][k],peso_v);
                    }
                }
            }
            return accum;
        }

};



#endif