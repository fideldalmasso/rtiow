#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"
class material{
public:
	refleja(const rayo& rayo_incidente, const registro_choque& registro, color& atenuacion, rayo& rayo_reflejado) const = 0;
};




#endif
