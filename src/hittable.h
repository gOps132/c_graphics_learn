#ifndef __HITTABLE_H__
#define __HITTABLE_H__

#include "rt_utility.h"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;

    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.get_direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

typedef struct hit_record hit_record;

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // __HITTABLE_H__