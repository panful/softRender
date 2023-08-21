#pragma once

#include <array>
#include <cmath>
#include <iostream>

class vec3
{
public:
    constexpr vec3() noexcept = default;

    constexpr vec3(double v) noexcept
        : _e { v, v, v }
    {
    }

    constexpr vec3(double x, double y, double z) noexcept
        : _e { x, y, z }
    {
    }

    std::array<double, 3> e() const noexcept
    {
        return _e;
    }

    [[nodiscard]] constexpr double x() const noexcept
    {
        return _e.at(0);
    }

    [[nodiscard]] constexpr double y() const noexcept
    {
        return _e.at(1);
    }

    [[nodiscard]] constexpr double z() const noexcept
    {
        return _e.at(2);
    }

    [[nodiscard]] constexpr double operator[](size_t i) const
    {
        return _e.at(i);
    }

    [[nodiscard]] constexpr vec3 operator-() const noexcept
    {
        return vec3(-_e[0], -_e[1], -_e[2]);
    }

    [[nodiscard]] constexpr double& operator[](size_t i)
    {
        return _e.at(i);
    }

    [[nodiscard]] constexpr vec3& operator*=(const double t) noexcept
    {
        _e.at(0) *= t;
        _e.at(1) *= t;
        _e.at(2) *= t;
        return *this;
    }

    constexpr vec3& operator+=(const vec3& v) noexcept
    {
        _e.at(0) += v.x();
        _e.at(1) += v.y();
        _e.at(2) += v.z();
        return *this;
    }

    [[nodiscard]] constexpr vec3& operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    [[nodiscard]] double length() const noexcept
    {
        return std::hypot(_e[0], _e[1], _e[2]);
    }

    [[nodiscard]] constexpr double length_squared() const noexcept
    {
        return _e[0] * _e[0] + _e[1] * _e[1] + _e[2] * _e[2];
    }

    inline static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    void write_color(std::ostream& out, int samples_per_pixel) const noexcept
    {
        // Divide the color total by the number of samples.
        auto scale = 1.0 / samples_per_pixel;
        auto r     = sqrt(scale * _e[0]);
        auto g     = sqrt(scale * _e[1]);
        auto b     = sqrt(scale * _e[2]);

        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' ' << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
    }

    friend std::ostream& operator<<(std::ostream& out, const vec3& v)
    {
        return out << v._e[0] << ' ' << v._e[1] << ' ' << v._e[2];
    }

    friend vec3 operator+(const vec3& u, const vec3& v)
    {
        return vec3(u._e[0] + v._e[0], u._e[1] + v._e[1], u._e[2] + v._e[2]);
    }

    friend vec3 operator-(const vec3& u, const vec3& v)
    {
        return vec3(u._e[0] - v._e[0], u._e[1] - v._e[1], u._e[2] - v._e[2]);
    }

    friend vec3 operator*(const vec3& u, const vec3& v)
    {
        return vec3(u._e[0] * v._e[0], u._e[1] * v._e[1], u._e[2] * v._e[2]);
    }

    friend vec3 operator*(double t, const vec3& v)
    {
        return vec3(t * v._e[0], t * v._e[1], t * v._e[2]);
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
        return u._e[0] * v._e[0] + u._e[1] * v._e[1] + u._e[2] * v._e[2];
    }

    friend vec3 cross(const vec3& u, const vec3& v)
    {
        return vec3(u._e[1] * v._e[2] - u._e[2] * v._e[1], u._e[2] * v._e[0] - u._e[0] * v._e[2], u._e[0] * v._e[1] - u._e[1] * v._e[0]);
    }

    friend vec3 unit_vector(vec3 v)
    {
        return v / v.length();
    }

private:
    std::array<double, 3> _e { 0.0, 0.0, 0.0 };
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

// 反射
vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

// 折射
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    auto cos_theta      = dot(-uv, n);
    vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_perp     = -sqrt(1.0 - r_out_parallel.length_squared()) * n;
    return r_out_parallel + r_out_perp;
}

double schlick(double cosine, double ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0      = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// 从一个单位小圆盘射出光线
vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}