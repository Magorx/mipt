#include "renderables.h"


r_Circle::r_Circle(Vec3d pos, double rad, SmartColor *color) :
RenderableObject(pos, color),
rad(rad)
{}

void r_Circle::render(Renderer *renderer) {
    renderer->draw_circle(position, rad, color->rgb());
}

r_Square::r_Square(Vec3d pos, double size, SmartColor *color) :
RenderableObject(pos, color),
size(size)
{}

void r_Square::render(Renderer *renderer) {
    renderer->draw_square(position, size, color->rgb());
}

r_Rectangle::r_Rectangle(Vec2d pos, Vec2d size, SmartColor *color) :
RenderableObject(pos, color),
size(size)
{}

void r_Rectangle::render(Renderer *renderer) {
    renderer->draw_rectangle(position, size, color->rgb());
}

r_Line::r_Line(Vec3d p1, Vec3d p2, SmartColor *color) :
RenderableObject(p1, color),
p1(p1),
p2(p2)
{}

void r_Line::render(Renderer *renderer) {
    renderer->draw_line(p1, p2, color->rgb());
}
