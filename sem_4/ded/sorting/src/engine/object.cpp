#include "object.h"


Object::Object(RenderableObject *texture, SolidBody  *solid_body):
texture(texture),
solid_body(solid_body),
del_logic(false)
{}

Object::~Object() {
    obj_delete();
}

void Object::obj_delete() {
    del_tickable = true;
    del_logic = true;

    if (texture) {
        texture->del();
        texture = nullptr;
    }

    if (solid_body) {
        solid_body->del();
        solid_body = nullptr;
    }
}
