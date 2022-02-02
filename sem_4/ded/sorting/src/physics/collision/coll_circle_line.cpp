#include "coll_circle_line.h"


CircleLineCollision::CircleLineCollision(sb_Circle *first, sb_Line *second) :
Collision(first, second)
{}

Collision *CircleLineCollision::collide(sb_Circle *first, sb_Line *second) {
    if (do_collide(first, second)) {
        return new CircleLineCollision(first, second);
    } else {
        return nullptr;
    }
}

void CircleLineCollision::solve() {
    // printf("bm\n");
    sb_Circle *c_first = (sb_Circle*) first;
    sb_Line *c_second = (sb_Line*) second;

    Vec3d vec = c_second->get_position() - c_first->get_position();
    Vec3d vel = c_first->get_velocity();
    Vec3d norm = c_second->get_normal().orient(vec);
    norm.normalize();

    double projection = vel.dot(norm);

    if (projection < 0) return;

    vel = vel - 2 * norm * projection;

    c_first->set_velocity(vel);
}
