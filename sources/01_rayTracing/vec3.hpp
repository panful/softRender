#pragma once

#include <array>
#include <cmath>
#include <iostream>

class vec3
{
public:
    constexpr vec3() noexcept = default;

    constexpr vec3(double x, double y, double z) noexcept
        : e { x, y, z }
    {
    }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    [[nodiscard]] constexpr double x() const noexcept
    {
        return e.at(0);
    }

    [[nodiscard]] constexpr double y() const noexcept
    {
        return e.at(1);
    }

    [[nodiscard]] constexpr double z() const noexcept
    {
        return e.at(2);
    }

    [[nodiscard]] constexpr vec3 operator-() const noexcept
    {
        return vec3(-e[0], -e[1], -e[2]);
    }

    [[nodiscard]] constexpr double& operator[](size_t i)
    {
        return e.at(i);
    }

    [[nodiscard]] constexpr vec3& operator*=(const double t) noexcept
    {
        e.at(0) *= t;
        e.at(1) *= t;
        e.at(2) *= t;
        return *this;
    }

    constexpr vec3& operator+=(const vec3& v) noexcept
    {
        e.at(0) += v.x();
        e.at(1) += v.y();
        e.at(2) += v.z();
        return *this;
    }

    [[nodiscard]] constexpr vec3& operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    [[nodiscard]] double length() const noexcept
    {
        return std::hypot(e[0], e[1], e[2]);
    }

    [[nodiscard]] constexpr double length_squared() const noexcept
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    void write_color(std::ostream& out, int samples_per_pixel) const noexcept
    {
        // Divide the color total by the number of samples.
        auto scale = 1.0 / samples_per_pixel;
        auto r     = sqrt(scale * e[0]);
        auto g     = sqrt(scale * e[1]);
        auto b     = sqrt(scale * e[2]);

        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' ' << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out, const vec3& v)
    {
        return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    }

    friend vec3 operator+(const vec3& u, const vec3& v)
    {
        return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
    }

    friend vec3 operator-(const vec3& u, const vec3& v)
    {
        return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
    }

    friend vec3 operator*(const vec3& u, const vec3& v)
    {
        return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
    }

    friend vec3 operator*(double t, const vec3& v)
    {
        return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    friend vec3 operator*(const vec3& v, double t)
    {
        return t * v;
    }

    friend vec3 operator/(vec3 v, double t)
    {
        return (1 / t) * v;
    }

    friend double dot(const vec3& u, const vec3& v)
    {
        return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
    }

    friend vec3 cross(const vec3& u, const vec3& v)
    {
        return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    }

    friend vec3 unit_vector(vec3 v)
    {
        return v / v.length();
    }

private:
    std::array<double, 3> e { 0.0, 0.0, 0.0 };
};

// 在球体内生成一个随机点
vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

// lambertian
// 使用极坐标在球体生成一个随机点
vec3 random_unit_vector()
{
    auto a = random_double(0.0, 2 * pi);
    auto z = random_double(-1.0, 1.0);
    auto r = sqrt(1 - z * z);
    auto p = vec3(r * cos(a), r * sin(a), z);
    return p;
}

vec3 random_in_hemisphere(const vec3& normal)
{
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}