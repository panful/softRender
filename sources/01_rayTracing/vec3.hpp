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

    [[nodiscard]] constexpr vec3& operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    [[nodiscard]] double length() const noexcept
    {
        return std::hypot(e[0], e[1], e[2]);
    }

    void write_color(std::ostream& out) const noexcept
    {
        out << static_cast<int>(255.999 * e[0]) << ' ' << static_cast<int>(255.999 * e[1]) << ' ' << static_cast<int>(255.999 * e[2]) << '\n';
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
