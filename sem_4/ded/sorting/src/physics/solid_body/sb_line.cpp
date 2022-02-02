#include "sb_line.h"
#include "collision/coll_circle_line.h"

sb_Line::sb_Line(const Vec3d &position, const Vec3d &normal) :
SolidBody(SolidBodyType::Line, position, {0, 0, 0}, 1),
normal(normal)
{}

Collision *sb_Line::collide(SolidBody *other) {
    switch(other->get_type()) {
        case SolidBodyType::Circle :
        case SolidBodyType::Square :
            return CircleLineCollision::collide((sb_Circle*) other, this);

        default :
            return nullptr;
    }
}
