#ifndef RENDERABLES_COLLECTION_H
#define RENDERABLES_COLLECTION_H

#include "visual/renderer/renderable_object.h"
#include "visual/color/smart_color.h"

#include "utils/vec3d.h"
#include "utils/vec2d.h"


struct r_Circle : RenderableObject {
    double rad;

    r_Circle(Vec3d pos, double rad, SmartColor *color);

    void render(Renderer *renderer) override;
};

struct r_Square : RenderableObject {
    double size;

    r_Square(Vec3d pos, double size, SmartColor *color);

    void render(Renderer *renderer) override;
};

struct r_Rectangle : RenderableObject {
    Vec2d size;

    r_Rectangle(Vec2d pos, Vec2d size, SmartColor *color);

    void render(Renderer *renderer) override;
};

struct r_Line : RenderableObject {
    Vec3d p1;
    Vec3d p2;

    r_Line(Vec3d p1, Vec3d p2, SmartColor *color);

    void render(Renderer *renderer) override;
};


#endif // RENDERABLES_COLLECTION_H