#ifndef SB_CIRCLE_H
#define SB_CIRCLE_H

#include "solid_body.h"

class sb_Circle : public SolidBody {
    double radius;

public:
    sb_Circle(const Vec3d &position, const double radius, const double mass=1);

    Collision *collide(SolidBody *other);

    inline double get_radius() const { return radius; }
    inline void set_radius(const double radius_) { radius = radius_; }
};

#endif // SB_CIRCLE_H
