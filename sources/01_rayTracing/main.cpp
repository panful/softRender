#include "ray.hpp"

bool hit_sphere(const vec3& center, double radius, const ray& r)
{
    vec3 oc           = r.origin() - center;
    auto a            = dot(r.direction(), r.direction());
    auto b            = 2.0 * dot(oc, r.direction());
    auto c            = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

vec3 ray_color(const ray& r)
{
    // 如果射线击中球体，则返回红色
    if (hit_sphere(vec3(0., 0., -1.), 0.5, r))
    {
        return vec3(1.0, 0.0, 0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // 将t归一化到[0,1]，就是将y从0递增到1，实现竖直方向上的渐变效果
    auto t = 0.5 * (unit_direction.y() + 1.0);
    // 线性插值
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
    const int image_width  = 200;
    const int image_height = 100;

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0); // 屏幕的左下角坐标
    vec3 horizontal(4.0, 0.0, 0.0);           // 水平跨度x
    vec3 vertical(0.0, 2.0, 0.0);             // 垂直跨度y
    vec3 origin(0.0, 0.0, 0.0);               // 相机位置
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / image_width;
            auto v = double(j) / image_height;
            // 从左下角一直遍历
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 color = ray_color(r);
            color.write_color(std::cout);
        }
    }

    std::cerr << "\nDone.\n";
}