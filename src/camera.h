#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "rt_utility.h"

class Camera
{
private:
    int image_width = 1000;

    double aspect_ratio = 16 / 9;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    double viewport_height  = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    vec3 origin = point3(0, 0, 0);
    vec3 horizontal = vec3(viewport_width, 0, 0);
    vec3 vertical = vec3(0, viewport_height, 0);
    vec3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

public:
    Camera(
        int p_image_width,
        double p_aspect_ratio,
        double p_viewport_height,
        double p_focal_length,
        vec3 p_origin
    )
    : 
        image_width(p_image_width), 
        aspect_ratio(p_aspect_ratio),
        viewport_height(p_viewport_height),
        focal_length(p_focal_length),
        origin(p_origin)
    {
        
    }
    ~Camera() {}
    
	ray get_ray(double u, double v) const {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

    // int get_image_width() { return image_width; }

    // double get_aspect_ratio() { return aspect_ratio; }
    // int get_image_height() { return image_height; }

    // double get_viewport_height() { return viewport_height; }
    // double get_viewport_width() { return viewport_width; }
    // double get_focal_length() { return focal_length; }

    vec3 get_origin() { return origin; }
    vec3 get_horizontal() { return horizontal; }
    vec3 get_vertical() { return vertical; }
    vec3 get_lower_left_corner() { return lower_left_corner; }
};

#endif // __CAMERA_H__