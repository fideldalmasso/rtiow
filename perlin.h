#ifndef PERLIN_H
#define PERLIN_H
#include "rtweekend.h"


class perlin{
    public:
        perlin(){
            arreglo_aleatorio = new double[cant_puntos];
            for(int i = 0; i< cant_puntos; ++i){
                arreglo_aleatorio[i] = double_aleatorio();
            }

            perm_x = perlin_generar_permutacion();
            perm_y = perlin_generar_permutacion();
            perm_z = perlin_generar_permutacion();

        }

        ~perlin(){
            delete[] arreglo_aleatorio;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        double ruido(const vec3& p) const{


            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            u = u*u*(3-2*u);
            v = v*v*(3-2*v);
            w = w*w*(3-2*w);


            //& == AND
            //^ == XOR

            auto i = static_cast<int>(floor(p.x()));
            auto j = static_cast<int>(floor(p.y()));
            auto k = static_cast<int>(floor(p.z()));

            double c[2][2][2];

            for(int di=0; di<2; di++){
                for(int dj=0; dj<2; dj++){
                    for(int dk=0; dk<2; dk++){
                        c[di][dj][dk] = arreglo_aleatorio[perm_x[(i+di)&255] ^ perm_y[(j+dj)&255] ^ perm_z[(k+dk)&255]];
                    }
                }
            }
            
            return interpolacion_trilinear(c,u,v,w);
        }

    private:
        static const int cant_puntos = 256;
        double* arreglo_aleatorio; //arreglo con 256 doubles aleatorios
        int* perm_x;             //arreglo con numeros del 0 a 255, desordenados
        int* perm_y;             //lo mismo aca
        int* perm_z;             //lo mismo aca

        static int* perlin_generar_permutacion(){
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

        static double interpolacion_trilinear(double c[2][2][2], double u, double v, double w){
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

};



#endif