#include "objects.h"

o_Ball::o_Ball(Vec3d pos, double rad, SmartColor *color, double mass):
Object(new r_Circle(pos, rad, color),
       new sb_Circle(pos, rad, mass))
{}

void o_Ball::tick(const double, const double) {
    texture->set_position(solid_body->get_position());
}

o_Rect::o_Rect(Vec3d pos, double size, SmartColor *color, double mass):
Object(new r_Square(pos, size, color),
       new sb_Circle(pos, size, mass))
{
       solid_body->set_type(SolidBodyType::Square);
}

void o_Rect::tick(const double, const double) {
    texture->set_position(solid_body->get_position());
}

o_Line::o_Line(Vec3d p1, Vec3d p2, SmartColor *color):
Object(new r_Line(p1, p2, color),
       new sb_Line(p1, (p2 - p1).roted_90_xy()))
{}

void o_Line::tick(const double, const double) {
}
