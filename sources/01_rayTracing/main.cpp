#include "ray.hpp"

double hit_sphere(const vec3& center, double radius, const ray& r)
{
    vec3 oc = r.origin() - center;
    auto a  = dot(r.direction(), r.direction());
    auto b  = 2.0 * dot(oc, r.direction());
    auto c  = dot(oc, oc) - radius * radius;

    // 求根公式，大于0，则有两个交点，等于0则一个交点，小于0则没有交点
    auto discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        // t = (-b ± sqrt(discriminant)) / (2.0 * a)
        // 此处返回t的较小值，即距离相机更近的交点对应的t
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

vec3 ray_color(const ray& r)
{
    auto t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        // 球心到球面的连线方向就是球面上该点的法线
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        // 保证输出的颜色值不小于0.0
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // 将t归一化到[0,1]，就是将y从0递增到1，实现竖直方向上的渐变效果
    t = 0.5 * (unit_direction.y() + 1.0);
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
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
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

    std::clog << "\nDone.\n";
}