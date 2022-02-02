#include "solid_body.h"
#include "sb_circle.h"
#include "sb_line.h"

SolidBody::SolidBody(SolidBodyType type, Vec3d position, Vec3d velocity, double mass):
type(type),
position(position),
velocity(velocity),
mass(mass),
del_solid(false)
{}

Collision *SolidBody::collide(SolidBody *other) {
    switch (get_type())
    {
    case SolidBodyType::Circle:
    case SolidBodyType::Square:
        return ((sb_Circle*) this)->collide(other);
    
    case SolidBodyType::Line:
        return ((sb_Line*) this)->collide(other);
    
    default:
        return nullptr;
    }
}
