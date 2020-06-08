#ifndef COMMON_UTILITY_H
#define COMMON_UTILITY_H

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <memory>

//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions
inline double degrees_to_radians(double degrees){
    return degrees * pi/180;
}

//Random Number Gen
inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

//Common Headers
#include "ray.h"
#include "vec3.h"

#endif //COMMON_UTILITY_H