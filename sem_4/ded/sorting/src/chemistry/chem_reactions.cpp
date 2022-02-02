// BAD CODE HERE, IM SORRY

#include "chem_reactions.h"

void reaction_circle_circle(Object *first, Object *second, MogaEngine *engine) {
    sb_Circle *sb_first  = (sb_Circle*) first->get_solid_body();
    sb_Circle *sb_second = (sb_Circle*) second->get_solid_body();

    if (sb_first->get_mass() < sb_second->get_mass()) {
        std::swap(first, second);
        std::swap(sb_first, sb_second);
    }

    Vec3d vel_first = sb_first->get_velocity();
    Vec3d vel_second = sb_second->get_velocity();
    
    double mass_first = sb_first->get_mass();
    double mass_second = sb_second->get_mass();

    double energy = mass_first * vel_first.len_squared() + mass_second * vel_second.len_squared();
    double vel_module = sqrt(energy / (mass_first + mass_second));

    Vec3d vel = (vel_first * mass_first + vel_second * mass_second) / (mass_first + mass_second);
    vel.normalize();
    vel *= vel_module;

    Color col_first = first->get_texture()->get_color()->rgb();
    Color col_second = second->get_texture()->get_color()->rgb();
    Color col = (col_first * mass_first + col_second * mass_second) / (mass_first + mass_second);

    // sb_first->set_velocity(vel);
    // sb_first->set_type(SolidBodyType::Square);

    SmartColor *color = new SmartColor(col); 

    Object *rect = new o_Rect(sb_first->get_position(),
                              sb_first->get_radius() + sb_second->get_radius(),
                              color,
                              mass_first + mass_second);
    rect->get_solid_body()->set_velocity(vel);

    engine->add_object(rect);
    engine->add_tickable(color);

    first->obj_delete();
    second->obj_delete();
}

void reaction_circle_square(Object *first, Object *second, MogaEngine *) {
    sb_Circle *sb_first  = (sb_Circle*) first->get_solid_body();
    sb_Circle *sb_second = (sb_Circle*) second->get_solid_body();

    Vec3d vel_first = sb_first->get_velocity();
    Vec3d vel_second = sb_second->get_velocity();
    
    double mass_first = sb_first->get_mass();
    double mass_second = sb_second->get_mass();

    double energy = mass_first * vel_first.len_squared() + mass_second * vel_second.len_squared();
    double vel_module = sqrt(energy / (mass_first + mass_second));

    Vec3d vel = (vel_first * mass_first + vel_second * mass_second) / (mass_first + mass_second);
    vel.normalize();
    vel *= vel_module;

    sb_second->set_velocity(vel);
    sb_second->set_mass(sb_second->get_mass() + sb_first->get_mass());
    sb_second->set_radius(sb_second->get_radius() + sb_first->get_radius());

    r_Square *sq = (r_Square*) second->get_texture();
    sq->size = sb_second->get_radius();

    first->obj_delete();
}

void reaction_square_circle(Object *first, Object *second, MogaEngine *engine) {
    reaction_circle_square(second, first, engine);
}

void reaction_square_square(Object *first, Object *second, MogaEngine *engine) {
    sb_Circle *sb_first  = (sb_Circle*) first->get_solid_body();
    sb_Circle *sb_second = (sb_Circle*) second->get_solid_body();

    if (sb_first->get_mass() < sb_second->get_mass()) {
        std::swap(first, second);
        std::swap(sb_first, sb_second);
    }

    double mass = sb_first->get_mass() + sb_second->get_mass();
    // double radius = sb_first->get_radius() + sb_second->get_radius();

    int balls_cnt = mass;

    Vec3d rndvec(0, 0, 0);
    rndvec.set(0, rand() - RAND_MAX / 2);
    rndvec.set(1, rand() - RAND_MAX / 2);
    rndvec.normalize();

    Vec3d vel_first = sb_first->get_velocity();
    Vec3d vel_second = sb_second->get_velocity();
    
    double mass_first = sb_first->get_mass();
    double mass_second = sb_second->get_mass();

    double energy = mass_first * vel_first.len_squared() + mass_second * vel_second.len_squared();
    double vel_module = sqrt(energy / balls_cnt);

    Color col_first = first->get_texture()->get_color()->rgb();
    Color col_second = second->get_texture()->get_color()->rgb();
    Color col = (col_first * mass_first + col_second * mass_second) / (mass_first + mass_second);

    for (int i = 0; i < balls_cnt; ++i) {
        double angle = 2.0 * M_PI / balls_cnt * i;
        Vec3d dpos = rotz(rndvec, angle);
        Vec3d pos = sb_first->get_position() + dpos * balls_cnt * 2.5;

        SmartColor *color = nullptr;
        if (rand() % 10) {
            color = new SmartColor(col);
        } else {
            color = new SmartColor(randcolor());
        }
        o_Ball *ball = new o_Ball(pos, 5, color, 1);
        engine->add_object(ball);
        engine->add_tickable(color);

        Vec3d vel = Vec3d::random_unit();
        vel.set(2, 0);
        vel.normalize();
        vel += dpos.normal() * 2;
        vel.normalize();
        vel *= vel_module;
        ball->get_solid_body()->set_velocity(vel);
    }

    first->obj_delete();
    second->obj_delete();
}

typedef void (*ReactionFunc)(Object *first, Object *second, MogaEngine *engine);

void reaction(Object *first, Object *second, MogaEngine *engine) {
    static ReactionFunc reactions[2][2] = {
        {reaction_circle_circle, reaction_circle_square},
        {reaction_square_circle, reaction_square_square}
    };

    SolidBody *sb_first  = first->get_solid_body();
    SolidBody *sb_second = second->get_solid_body();

    if (sb_first->get_type() > SolidBodyType::Square || sb_second->get_type() > SolidBodyType::Square) {
        return;
    }

    reactions[(int) sb_first->get_type()][(int) sb_second->get_type()](first, second, engine);
}
