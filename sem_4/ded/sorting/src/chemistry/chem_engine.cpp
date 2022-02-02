#include "chem_engine.h"
#include "chem_reactions.h"

ChemEngine::ChemEngine( const char  *window_name,
                        const size_t screen_width,
                        const size_t screen_height,
                        const size_t pixel_size):
MogaEngine(window_name, screen_width, screen_height, pixel_size),
to_compute_chemistry(false)
{}

void ChemEngine::physics_tick() {
	physics->tick(PHYSICS_TIME_STEP * physics_time_multiplier, physics_current_time);
    
    if (to_compute_chemistry) reactions_process();

    // printf("objs: %d\n", objects.size());
}

void ChemEngine::reactions_process() {
    int obj_cnt = objects.size();

    for (int i = 0; i < obj_cnt; ++i) {
        if (objects[i]->del_logic) {
            continue;
        }
        Object *obj_1 = objects[i];

        for (int j = i + 1; j < obj_cnt; ++j) {
            if (objects[i]->del_logic) {
                break;
            }
            if (objects[j]->del_logic) {
                continue;
            }
            Object *obj_2 = objects[j];

            Collision *collision = obj_1->get_solid_body()->collide(obj_2->get_solid_body());
            if (!collision) {
                continue;
            } else {
                delete collision;
            }

            reaction(obj_1, obj_2, this);
        }
    }
}
