#include "physics_engine.h"


PhysicsEngine::PhysicsEngine():
tickables(),
solids(),
collisions()
{}

PhysicsEngine::~PhysicsEngine() {
    for (auto obj : tickables) {
        delete obj;
    }

    for (auto obj : solids) {
        delete obj;
    }

    for (auto obj : collisions) {
        delete obj;
    }
}

bool PhysicsEngine::add_phys_tickable(PhysTickable *object) {
    tickables.push_back(object);
    return true;
}

bool PhysicsEngine::add_solid(SolidBody *object) {
    solids.push_back(object);
    return true;
}

// void PhysicsEngine::global_affects(const double dt) {
// 	//printf("===grav===\n");
// 	for (int i = 0; i < solids.size(); ++i) {
// 		SolidBody &obj = *solids[i];
// 		PhysicPoint &mc = obj.mass_center;

// 		mc.apply_impulse(gravity * mc.mass * dt);

// 		mc.velocity    *= ENERGY_LOSS_COEF;
// 		mc.angle_speed *= ENERGY_LOSS_COEF;

// 		if (mc.velocity.len() > MAX_VELOCITY_MODULE) {
// 			if (mc.velocity.len() > MAX_VELOCITY_MODULE) {
// 				mc.velocity.normalize();
// 				mc.velocity *= MAX_VELOCITY_MODULE * 1.5;
// 			}
// 			mc.velocity *= 0.25;
// 		}

// 		if (fabs(mc.angle_speed) > MAX_ANGLE_SPEED) {
// 			if (mc.angle_speed > MAX_ANGLE_SPEED * 2) {
// 				mc.angle_speed = MAX_ANGLE_SPEED * 1.5 * vec2d_sign(mc.angle_speed);
// 			}
// 			mc.angle_speed *= 0.25;
// 		}
// 	}
// }

void PhysicsEngine::physics_tick(const double dt) {
    int deleted_cnt = 0;
    for (size_t i = 0; i < tickables.size(); ++i) {
        if (tickables[i]->del_phys_tickable) {
            ++deleted_cnt;
            continue;
        }

        tickables[i]->physics_tick(dt);
    }

    if (deleted_cnt > tickables.size() * PHYS_TICKABLE_BUFFER_REFRESH_COEF && deleted_cnt) {
        size_t next_alive = 0;
        size_t i = 0;
        size_t tickables_cnt = tickables.size();
        for (; i < tickables_cnt; ++i) {
            if (!tickables[i] || tickables[i]->del_phys_tickable) {
                for (next_alive = std::max(i + 1, next_alive); 
                        next_alive < tickables_cnt && tickables[next_alive]->del_phys_tickable;
                        ++next_alive); // find next alive object

                if (next_alive < tickables_cnt) {
                    if (tickables[i]) {
                        delete tickables[i];
                    }

                    tickables[i] = tickables[next_alive];
                    tickables[next_alive] = nullptr;
                    ++next_alive;
                }
            }
        }
        tickables.resize(tickables.size() - deleted_cnt);
    }

    deleted_cnt = 0;
    for (size_t i = 0; i < solids.size(); ++i) {
        if (solids[i]->del_solid) {
            ++deleted_cnt;
            continue;
        }

        solids[i]->solid_tick(dt);
    }

    if (deleted_cnt > solids.size() * PHYS_TICKABLE_BUFFER_REFRESH_COEF && deleted_cnt) {
        size_t next_alive = 0;
        size_t i = 0;
        size_t solids_cnt = solids.size();
        for (; i < solids_cnt; ++i) {
            if (!solids[i] || solids[i]->del_solid) {
                for (next_alive = std::max(i + 1, next_alive); 
                        next_alive < solids_cnt && solids[next_alive]->del_solid;
                        ++next_alive); // find next alive object

                if (next_alive < solids_cnt) {
                    if (solids[i]) {
                        delete solids[i];
                    }

                    solids[i] = solids[next_alive];
                    solids[next_alive] = nullptr;
                    ++next_alive;
                }
            }
        }
        solids.resize(solids.size() - deleted_cnt);
    }
}

void PhysicsEngine::tick(const double dt, const double) {
    physics_tick(dt);

    update_collisions();
    solve_collisions();
}

void PhysicsEngine::update_collisions() {
    for (auto coll : collisions) delete coll;
    collisions.clear();
    
    for (size_t i = 0; i < solids.size(); ++i) {
        if (solids[i]->del_solid) continue;

        for (size_t j = i + 1; j < solids.size(); ++j) {
            if (solids[j]->del_solid) continue;

            Collision *collision = solids[i]->collide(solids[j]);
            if (collision) {
                collisions.push_back(collision);
            }
        }
    }
}

void PhysicsEngine::solve_collisions() {
    for (size_t i = 0; i < collisions.size(); ++i) {
        Collision *collision = collisions[i];
        if (!collision) {
            continue;
        }

        collision->solve();
    }
}
