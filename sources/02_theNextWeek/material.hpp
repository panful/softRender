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

        scattered = ray(rec.p, scatter_direction, r_in.time());

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

// 绝缘体材质，只会发生折射
class dielectric : public material
{
public:
    dielectric(double ri)
        : ref_idx(ri)
    {
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        attenuation           = vec3(1.0, 1.0, 1.0); // 光线衰减为1，即不衰减
        double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta    = ffmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta    = sqrt(1.0 - cos_theta * cos_theta);

        // 无法发生折射的时候，发生反射
        if (etai_over_etat * sin_theta > 1.0)
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered      = ray(rec.p, reflected);
            return true;
        }

        // 当从很窄的监督去看玻璃窗，它会变为一面镜子，发生反射
        double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_double() < reflect_prob)
        {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered      = ray(rec.p, reflected);
            return true;
        }

        // 发生折射
        vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered      = ray(rec.p, refracted);
        return true;
    }

public:
    double ref_idx;
};
