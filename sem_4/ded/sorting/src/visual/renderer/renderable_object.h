#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H


#include "renderable.h"
#include "visual/color/smart_color.h"
#include "utils/vec3d.h"


class RenderableObject : public Renderable {
protected:
    Vec3d position;
    SmartColor *color;

public:
    RenderableObject() : position(Vec3d{0, 0, 0}), color(nullptr) {}
    RenderableObject(Vec3d position, SmartColor *color) : position(position), color(color) {}

    inline Vec3d get_position() const { return position; }
    inline void set_position(const Vec3d &position_) { position = position_; }

    inline SmartColor *get_color() { return color; }
    inline void set_color(SmartColor *color_) { color = color_; }
};

#endif // RENDERABLE_OBJECT_H
