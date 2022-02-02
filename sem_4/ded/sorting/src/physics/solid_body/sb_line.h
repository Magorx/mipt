#ifndef SB_LINE_H
#define SB_LINE_H

#include "solid_body.h"

class sb_Line : public SolidBody {
    Vec3d normal;

public:
    sb_Line(const Vec3d &position, const Vec3d &normal);

    Collision *collide(SolidBody *other);

    inline Vec3d get_normal() const { return normal; }
};

#endif // SB_LINE_H
