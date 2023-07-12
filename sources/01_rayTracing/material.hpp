#pragma once

#include "rtweekend.hpp"

struct hit_record;

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

// 漫反射材质
class lambertian : public material
{
public:
    lambertian(const vec3& a)
        : albedo(a)
    {
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        // 散射方向
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered              = ray(rec.p, scatter_direction);
        // 衰减，光线变暗
        attenuation = albedo;
        // 是否发生散射，
        // 也可以使用一个概率p来决定是否发生散射（没有散射光线则直接消失）发生散射则光线的衰减率变为albedo/p
        return true;
    }

public:
    vec3 albedo;
};

// 金属材质，发生反射
class metal : public material
{
public:
    metal(const vec3& a, double f)
        : albedo(a)
        , fuzz(f < 1 ? f : 1)
    {
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered      = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation    = albedo;
        return (dot(scattered.direction(), rec.normal) > 0); // dot<0我们认为吸收
    }

public:
    vec3 albedo;
    double fuzz; // 金属的模糊度（粗糙度），当fuzz等于0时不会产生模糊
};