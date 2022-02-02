#include "sb_circle.h"
#include "collision/coll_circle_circle.h"
#include "collision/coll_circle_line.h"

sb_Circle::sb_Circle(const Vec3d &position, const double radius, const double mass) :
SolidBody(SolidBodyType::Circle, position, {0, 0, 0}, mass),
radius(radius)
{}

Collision *sb_Circle::collide(SolidBody *other) {
    switch(other->get_type()) {
        case SolidBodyType::Circle :
        case SolidBodyType::Square : 
            return CircleCircleCollision::collide(this, (sb_Circle*) other);
        
        case SolidBodyType::Line :
            return CircleLineCollision::collide(this, (sb_Line*) other);

        default :
            return nullptr;
    }
}
