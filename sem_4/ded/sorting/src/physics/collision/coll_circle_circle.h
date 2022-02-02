#ifndef CIRCLE_CIRCLE_COLLISION
#define CIRCLE_CIRCLE_COLLISION

#include "collision.h"
#include "physics/solid_body/sb_circle.h"

class CircleCircleCollision : public Collision {
public:
    CircleCircleCollision(sb_Circle *first, sb_Circle *second);

    static inline bool do_collide(sb_Circle *first, sb_Circle *second) {
        double distance = (first->get_position() - second->get_position()).len();
        double radius_distance = first->get_radius() + second->get_radius();
        
        return radius_distance > distance;
    }

    void solve() override;

    static Collision *collide(sb_Circle *first, sb_Circle *second);
};

#endif // CIRCLE_CIRCLE_COLLISION