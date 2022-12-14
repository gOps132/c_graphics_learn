#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "hittable.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)
        const override;
        
public:
    point3 center;
    double radius;
};

#endif // __SPHERE_H__