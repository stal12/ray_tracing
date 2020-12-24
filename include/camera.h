#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"
#include "vec3.h"
#include "ray.h"

class camera {
public:

    camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist, double _time0 = 0, double _time1 = 0) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin_ = lookfrom;
        horizontal_ = focus_dist * viewport_width * u;
        vertical_ = focus_dist * viewport_height * v;
        lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(
            origin_ + offset,
            lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset,
            random_double(time0, time1)
        );
    }

private:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;  // shutter open/close times
};
#endif