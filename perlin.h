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

        double ruido(const punto3& p) const{

            //& == AND
            //^ == XOR
            
            // truncar el binario dejando solo los ultimos 8 digitos
            auto i = static_cast<int>(4*p.x()) & 255; //siempre un entero entre 0 y 255. 
            auto j = static_cast<int>(4*p.y()) & 255; //lo mismo aca
            auto k = static_cast<int>(4*p.z()) & 255; //lo mismo aca

            return arreglo_aleatorio[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
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
};



#endif