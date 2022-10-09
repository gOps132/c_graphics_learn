#include <iostream>
#include <fstream> 
#include <string>

#include "color.h"
#include "vec3.h"
#include "ray.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.get_origin() - center;
    auto a = r.get_direction().length_squared();
    auto half_b = dot(oc, r.get_direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    // If hits
    if (t > 0.0) {
        // front side of the sphere for now?
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
    vec3 unit_direction = unit_vector(r.get_direction());
    t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char** argv) 
{
    constexpr int image_width = 400;

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    std::cout << "Lower Left Corner: \n" 
            << "x = " << lower_left_corner.x() 
            << "\n" << "y = " << lower_left_corner.y() << "\n" 
            << "z = " << lower_left_corner.z() << "\n";

    std::ofstream output_file;
    output_file.open([&](){
        if (argc > 1) {
            const char * p1 = argv[1];
            return p1;
        } else {
            return "default.ppm";
        }
    }());
    output_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Render
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(output_file, pixel_color);
        }
    }

    output_file.close();
    std::cerr << "\nDone.\n";
    
    //test
    std::cout   << "ARGC COUNT: " << argc << "\n" 
                << "ARGV COUNT: " << argv[1];

    return 0;
}