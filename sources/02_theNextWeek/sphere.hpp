#pragma once

#include "hittable.hpp"
#include "rtweekend.hpp"

class sphere : public hittable
{
public:
    sphere() noexcept = default;

    sphere(vec3 cen, double r, shared_ptr<material> m)
        : center(cen)
        , radius(r)
        , mat_ptr(m)
    {
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        vec3 oc           = r.origin() - center;
        auto a            = r.direction().length_squared();
        auto half_b       = dot(oc, r.direction());
        auto c            = oc.length_squared() - radius * radius;
        auto discriminant = half_b * half_b - a * c;

        if (discriminant > 0)
        {
            auto root = sqrt(discriminant);
            auto temp = (-half_b - root) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t               = temp;
                rec.p               = r.at(rec.t);
                vec3 outward_normal = (rec.p - center) / radius;
                rec.set_face_normal(r, outward_normal);
                rec.mat_ptr = mat_ptr;
                return true;
            }
            temp = (-half_b + root) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t               = temp;
                rec.p               = r.at(rec.t);
                vec3 outward_normal = (rec.p - center) / radius;
                rec.set_face_normal(r, outward_normal);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        return false;
    }

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override
    {
        output_box = aabb(center - vec3(radius), center + vec3(radius));
        return true;
    }

public:
    vec3 center {};
    double radius { 0.0 };
    shared_ptr<material> mat_ptr;
};

class moving_sphere : public hittable
{
public:
    moving_sphere()
    {
    }

    moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double r, shared_ptr<material> m)
        : center0(cen0)
        , center1(cen1)
        , time0(t0)
        , time1(t1)
        , radius(r)
        , mat_ptr(m)
    {
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
    {
        vec3 oc     = r.origin() - center(r.time());
        auto a      = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c      = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;

        if (discriminant > 0)
        {
            auto root = sqrt(discriminant);

            auto temp = (-half_b - root) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t               = temp;
                rec.p               = r.at(rec.t);
                vec3 outward_normal = (rec.p - center(r.time())) / radius;
                rec.set_face_normal(r, outward_normal);
                rec.mat_ptr = mat_ptr;
                return true;
            }

            temp = (-half_b + root) / a;
            if (temp < t_max && temp > t_min)
            {
                rec.t               = temp;
                rec.p               = r.at(rec.t);
                vec3 outward_normal = (rec.p - center(r.time())) / radius;
                rec.set_face_normal(r, outward_normal);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        return false;
    }

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override
    {
        aabb box0(center(t0) - vec3(radius), center(t0) + vec3(radius));
        aabb box1(center(t1) - vec3(radius), center(t1) + vec3(radius));

        output_box = surrounding_box(box0, box1);
        return true;
    }

    vec3 center(double time) const
    {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }

private:
    vec3 center0, center1;
    double time0, time1;
    double radius;
    shared_ptr<material> mat_ptr;
};
