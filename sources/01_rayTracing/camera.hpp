#pragma once

#include "rtweekend.hpp"

class camera
{
public:
    // vfov: top to bottom, in degrees
    // lookfrom 摄像机位置
    // lookat 摄像机焦点
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect)
    {
        origin = lookfrom;
        vec3 u, v, w;

        auto theta       = degrees_to_radians(vfov);
        auto half_height = tan(theta / 2);
        auto half_width  = aspect * half_height;
        w                = unit_vector(lookfrom - lookat);
        u                = unit_vector(cross(vup, w));
        v                = cross(w, u);

        lower_left_corner = origin - half_width * u - half_height * v - w;

        horizontal = 2 * half_width * u;
        vertical   = 2 * half_height * v;
    }

    ray get_ray(double s, double t)
    {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};