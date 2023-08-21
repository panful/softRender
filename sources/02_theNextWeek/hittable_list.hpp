#pragma once

#include "hittable.hpp"
#include "rtweekend.hpp"
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable
{
public:
    hittable_list() noexcept = default;

    hittable_list(shared_ptr<hittable> object) noexcept
    {
        add(object);
    }

    void clear()
    {
        _objects.clear();
    }

    void add(shared_ptr<hittable> object)
    {
        _objects.push_back(object);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        hit_record temp_rec;
        bool hit_anything   = false;
        auto closest_so_far = t_max;

        for (const auto& object : _objects)
        {
            if (object->hit(r, t_min, closest_so_far, temp_rec))
            {
                hit_anything   = true;
                closest_so_far = temp_rec.t;
                rec            = temp_rec;
            }
        }

        return hit_anything;
    }

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override
    {
        if (_objects.empty())
        {
            return false;
        }

        aabb temp_box {};
        bool first_box { true };

        for (auto&& object : _objects)
        {
            if (!object->bounding_box(t0, t1, temp_box))
            {
                return false;
            }

            output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
            first_box  = false;
        }

        return true;
    }

    std::vector<shared_ptr<hittable>> objects() const
    {
        return _objects;
    }

private:
    std::vector<shared_ptr<hittable>> _objects;
};
