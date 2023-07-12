#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi       = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

inline double ffmin(double a, double b)
{
    return a <= b ? a : b;
}

inline double ffmax(double a, double b)
{
    return a >= b ? a : b;
}

// Common Headers
#include "ray.hpp"
#include "vec3.hpp"