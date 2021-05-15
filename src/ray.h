#ifndef __RAY_H__
#define __RAY_H__

#include "vec3.h"

class ray
{
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) 
        : m_origin(origin), m_direction(direction)
    {}
    
    point3 get_origin() const { return m_origin; }
    vec3 get_direction() const { return m_direction; }

    point3 at(double t) const {
        return m_origin + t * m_direction;
    }
private:
    point3 m_origin;
    vec3 m_direction;
};

#endif // __RAY_H__