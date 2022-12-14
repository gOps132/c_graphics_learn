#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"
#include "rt_utility.h"

void write_color_to_file(std::ostream &out, color &pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}

int write_color(color &pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return  static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << 16 |
			static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << 8 |
			static_cast<int>(255.999 * clamp(b, 0.0, 0.999));
}

#endif