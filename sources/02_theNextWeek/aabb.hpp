#pragma once

#include "rtweekend.hpp"

class aabb
{
public:
    constexpr aabb() noexcept = default;

    constexpr aabb(const vec3& a, const vec3& b) noexcept
        : _min(a)
        , _max(b)
    {
    }

    /// @brief
    /// @param r
    /// @param tmin 时间最小值
    /// @param tmax 时间最大值
    /// @return
    bool hit(const ray& r, double tmin, double tmax) const noexcept
    {
        for (size_t i = 0; i < 3; ++i)
        {
            auto t0 = ffmin((_min[i] - r.origin()[i]) / r.direction()[i], (_max[i] - r.origin()[i]) / r.direction()[i]);
            auto t1 = ffmax((_min[i] - r.origin()[i]) / r.direction()[i], (_max[i] - r.origin()[i]) / r.direction()[i]);

            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);

            if (tmax <= tmin)
            {
                return false;
            }
        }

        return true;
    }

    constexpr vec3 min() const noexcept
    {
        return _min;
    }

    constexpr vec3 max() const noexcept
    {
        return _max;
    }

private:
    vec3 _min;
    vec3 _max;
};

/// @brief 计算包围盒的包围盒
/// @param box0
/// @param box1
/// @return
aabb surrounding_box(aabb box0, aabb box1)
{
    vec3 small(ffmin(box0.min().x(), box1.min().x()), ffmin(box0.min().y(), box1.min().y()), ffmin(box0.min().z(), box1.min().z()));
    vec3 big(ffmax(box0.max().x(), box1.max().x()), ffmax(box0.max().y(), box1.max().y()), ffmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}