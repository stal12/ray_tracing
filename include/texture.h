#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include "color.h"
#include "perlin.h"
#include "my_stb_image.h"
#include "utility"


class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)) {}

    virtual color value(double u, double v, const vec3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() {}

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(_even), odd(_odd) {}

    checker_texture(color c1, color c2)
        : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const point3& p) const override {
        double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};

class noise_texture : public texture {
public:
    noise_texture(double sc) : scale(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
    }

public:
    perlin noise;
    double scale;
};

class turbo_texture : public texture {
public:
    turbo_texture(double sc, int dp) : scale(sc), depth(dp) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color(1, 1, 1) * noise.turb(scale * p, depth);
    }

public:
    perlin noise;
    double scale;
    int depth;
};

class marble_texture : public texture {
public:
    marble_texture(double sc, int dp) : scale(sc), depth(dp) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p, depth)));
    }

public:
    perlin noise;
    double scale;
    int depth;
};

class image_texture : public texture {
public:
    const static int values_per_pixel = 3;

    image_texture()
        : data(nullptr), width(0), height(0), values_per_scanline(0) {}

    image_texture(const char* filename) {
        int components_per_pixel = 3;

        unsigned char* byte_data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!byte_data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        values_per_scanline = values_per_pixel * width;

        data = new float[width * height * components_per_pixel];
        const double color_scale = 1.0 / 255.0;
        for (int i = 0; i < width * height * components_per_pixel; ++i) {
            data[i] = powf(byte_data[i] * color_scale, gamma);
        }

        free(byte_data);
    }

    ~image_texture() {
        delete[] data;
    }

    virtual color value(double u, double v, const vec3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (data == nullptr)
            return color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates should be less than 1.0
        if (i >= width)  i = width - 1;
        if (j >= height) j = height - 1;

        const double color_scale = 1.0 / 255.0;
        float* pixel = data + j * values_per_scanline + i * values_per_pixel;

        return color(pixel[0], pixel[1], pixel[2]);
    }

private:
    float* data;
    int width, height;
    int values_per_scanline;
};

#endif