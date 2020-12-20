#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.2.
    auto scale = 1.0 / samples_per_pixel;
    r = pow(scale * r, 1 / 2.2);
    g = pow(scale * g, 1 / 2.2);
    b = pow(scale * b, 1 / 2.2);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

color BGR(const color & c) {
    return color(c[2], c[1], c[0]);
}

#endif