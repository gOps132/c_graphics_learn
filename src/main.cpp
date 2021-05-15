#include <iostream>
#include <fstream> 
#include <string>

#include "color.h"
#include "vec3.h"


int main(int argc, char** argv) 
{
    constexpr int image_width = 256;
    constexpr int image_height = 256;

    // Render
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

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        // for (int i = 0; i < image_width; ++i) {
        //     auto r = double(i) / (image_width-1);
        //     auto g = double(j) / (image_height-1);
        //     auto b = 0.25;

        //     int ir = static_cast<int>(255.999 * r);
        //     int ig = static_cast<int>(255.999 * g);
        //     int ib = static_cast<int>(255.999 * b);

        //     std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        // }
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
            write_color(output_file, pixel_color);
        }
    }

    output_file.close();
    std::cerr << "\nDone.\n";
    
    //test
    std::cout << "argc" << argc << argv[1];
    return 0;
}