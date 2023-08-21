#pragma once

#include "hittable_list.hpp"

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis)
{
    aabb box_a {};
    aabb box_b {};

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    {
        std::cerr << "No bounding box in bvh_node constructor\n";
    }

    return box_a.min().e()[axis] < box_b.min().e()[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
    return box_compare(a, b, 2);
}

class bvh_node : public hittable
{
public:
    constexpr bvh_node() noexcept
    {
    }

    bvh_node(hittable_list& list, double time0, double time1)
        : bvh_node(list.objects(), 0, list.objects().size(), time0, time1)
    {
    }

    bvh_node(const std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1)
    {
        int axis           = random_int(0, 2);
        auto comparator    = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
        size_t object_span = end - start;

        if (object_span == 1)
        {
            _left = _right = objects[start];
        }
        else if (object_span == 2)
        {
            if (comparator(objects[start], objects[start + 1]))
            {
                _left  = objects[start];
                _right = objects[start + 1];
            }
            else
            {
                _left  = objects[start + 1];
                _right = objects[start];
            }
        }
        else
        {
            std::vector<shared_ptr<hittable>> temp_objects = objects;
            std::sort(temp_objects.begin() + start, temp_objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            _left     = make_shared<bvh_node>(temp_objects, start, mid, time0, time1);
            _right    = make_shared<bvh_node>(temp_objects, mid, end, time0, time1);
        }

        aabb box_left, box_right;

        if (!_left->bounding_box(time0, time1, box_left) || !_right->bounding_box(time0, time1, box_right))
        {
            std::cerr << "No bounding box in bvh_node constructor.\n";
        }

        _box = surrounding_box(box_left, box_right);
    }

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override
    {
        if (!_box.hit(r, tmin, tmax))
        {
            return false;
        }

        bool hit_left  = _left->hit(r, tmin, tmax, rec);
        bool hit_right = _right->hit(r, tmin, tmax, rec);

        return hit_left || hit_right;
    }

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override
    {
        output_box = _box;
        return true;
    }

private:
    shared_ptr<hittable> _left;
    shared_ptr<hittable> _right;
    aabb _box;
};