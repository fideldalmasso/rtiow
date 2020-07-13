#ifndef VEC3_H
#define VEC3_H


#include "rtweekend.h"
#include <iostream>


using std::sqrt;
using namespace std;

class vec3 {
public:
	vec3(): e{0,0,0}{};                                                         // miVec = vec3()
	
	vec3(double e0, double e1, double e2) : e{e0,e1,e2} {}                      // miVec = vec3(0.1,0.2,0.3)
	
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	
	vec3 operator-() const { return vec3(-e[0],-e[1],-e[2]);}                   // miVec1 = - miVec2
	
	double operator[](int i) const { return e[i];}                              // double x = miVector[3]
	
	double& operator[](int i) { return e[i];}                                   // miVector[3] = 10.0;
	
	vec3& operator+=(const vec3 &v){                                            //  miVec1+=miVec2;
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	
	vec3& operator*=(const double t){                                           // miVector*=3;
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	
	vec3& operator/=(const double t){                                           // miVector/=0.3;
		return  *this *= 1/t;
	}
	
	double longitud() const {                                                     //miVector.longitud()
		return sqrt(longitud_cuadrada());
	}
	
	double longitud_cuadrada() const{                                              //miVector.longitud_cuadrada()
		return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
	}
	
	inline static vec3 aleatorio(){
		return vec3(double_aleatorio(), double_aleatorio(), double_aleatorio());
	}
		
	inline static vec3 aleatorio(double min, double max){
		return vec3(double_aleatorio(min,max),double_aleatorio(min,max),double_aleatorio(min,max));
	}

public:
	double e[3];
};


//type aliases
using punto3 = vec3;
using color = vec3;
//--------------------------------------------------------
//vec3 utility functions
//para usar con cout << .... por eso usa ostream. 
//Se llama output operator overload
inline ostream& operator<<(ostream &out, const vec3 &v){                        //cout << miVec;
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v){                            //miVec1 + miVec2
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v){                            //miVec1 - miVec2
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v){                            //miVec1 * miVec2
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(double t, const vec3 &v){                                 // 0.1 * miVec
	return vec3(t*v[0], t*v[1], t*v[2]);
}

inline vec3 operator*(const vec3 &v, double t){                                 // miVec * 0.1
	return t * v;                                                               // llama al metodo de arriba
}

inline vec3 operator/(vec3 v, double t){
	return (1/t) * v;
}

inline double producto_punto(const vec3 &u, const vec3 &v){
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 producto_cruz(const vec3 &u, const vec3 &v){
	return vec3(u[1] * v[2] - u[2] * v[1], 
				u[2] * v[0] - u[0] * v[2],
				u[0] * v[1] - u[1] * v[0]);
}

inline vec3 vector_unitario(vec3 v) {
	return v / v.longitud();
}

//metodo1: vector dentro de esfera unitaria
vec3 vector_en_esfera_unitaria_aleatorio(){
	while(true){
		auto p = vec3::aleatorio(-1,1);
		if(p.longitud_cuadrada() >= 1)  continue;
		return p;
	}
}

//metodo2: vector sobre la superficie de esfera unitaria
vec3 vector_unitario_aleatorio(){
	auto angulo = double_aleatorio(0,2*pi);
	auto z = double_aleatorio(-1,1);
	auto r = sqrt(1 - z*z);
	return vec3(r*cos(angulo),r*sin(angulo),z);
}

//metodo3: vector dentro de semiesfera unitaria
vec3 vector_en_semiesfera_unitaria_aleatorio(const vec3& normal){
	vec3 vector_en_esfera = vector_en_esfera_unitaria_aleatorio();
	if(producto_punto(vector_en_esfera, normal) > 0.0) // el vector esta en la misma semiesfera que la normal
		return vector_en_esfera;
	else
		return -vector_en_esfera;
}
	

#endif

