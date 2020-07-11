#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinito = std::numeric_limits<double>::infinity();
const double pi = 3.141592653897932385;

inline double grados_a_radianes(double grados){
	return grados * pi / 180;
}
	
#include "rayo.h"
#include "vec3.h"

#endif
