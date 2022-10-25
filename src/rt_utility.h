#ifndef __RT_UTILITY_H__
#define __RT_UTILITY_H__

#include <cmath>
#include <limits>
#include <memory>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// clamps the value x to the range [min,max]:
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers
#include "ray.h"
#include "vec3.h"

#endif // __RT_UTILITY_H__