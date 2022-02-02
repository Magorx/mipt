#include "coll_circle_circle.h"

CircleCircleCollision::CircleCircleCollision(sb_Circle *first, sb_Circle *second) :
Collision(first, second)
{}

Collision *CircleCircleCollision::collide(sb_Circle *first, sb_Circle *second) {
    if (do_collide(first, second)) {
        return new CircleCircleCollision(first, second);
    } else {
        return nullptr;
    }
}

void CircleCircleCollision::solve() {
    sb_Circle *c_first = (sb_Circle*) first;
    sb_Circle *c_second = (sb_Circle*) second;

    Vec3d vel_first = c_first->get_velocity();
    Vec3d vel_second = c_second->get_velocity();

    Vec3d axis = c_second->get_position();
    axis -= c_first->get_position();
    axis.normalize();
    Vec3d axis_norm(-axis.y(), axis.x(), 0);

    Vec3d vel_mass_center = c_first->get_velocity() * c_first->get_mass();
    vel_mass_center += c_second->get_velocity() * c_second->get_mass();
    vel_mass_center /= (c_first->get_mass() + c_second->get_mass());

    vel_first -= vel_mass_center;
    vel_second -= vel_mass_center;

    vel_first =  {vel_first.dot(axis_norm) * c_first->get_mass(), vel_first.dot(axis) * c_first->get_mass(), 0};
    vel_second = {vel_second.dot(axis_norm) * c_second->get_mass(), vel_second.dot(axis) * c_second->get_mass(), 0};

    if (vel_first.y() < 0 || vel_second.y() > 0) return;

    double tmp_y = vel_first.y();
    vel_first.set(1, vel_second.y());
    vel_second.set(1, tmp_y);

    Vec3d new_velocity_first = (axis_norm * vel_first.x()) + (axis * vel_first.y());
    Vec3d new_velocity_second = (axis_norm * vel_second.x()) + (axis * vel_second.y());

    new_velocity_first  /= c_first->get_mass();
    new_velocity_second /= c_second->get_mass();

    new_velocity_first  += vel_mass_center;
    new_velocity_second += vel_mass_center;

    // new_velocity_first -= c_first->get_velocity();
    // new_velocity_second -= c_second->get_velocity();

    c_first->set_velocity(new_velocity_first);
    c_second->set_velocity(new_velocity_second);
}