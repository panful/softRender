#pragma once

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <numbers>
#include <random>

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

static std::default_random_engine randomEngine(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

inline double random_double()
{
    std::uniform_real_distribution<double> randomColor(0.0, 1.0);
    return randomColor(randomEngine);
}

inline double random_double(double min, double max)
{
    std::uniform_real_distribution<double> randomColor(min, max);
    return randomColor(randomEngine);
}

// Common Headers
#include "ray.hpp"
#include "vec3.hpp"