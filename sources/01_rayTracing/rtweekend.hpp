#pragma once

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <random>
#include <numbers>

// Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi       = std::numbers::pi_v<double>;

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

inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

inline double random_double(double min, double max)
{
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

// Common Headers
#include "ray.hpp"
#include "vec3.hpp"