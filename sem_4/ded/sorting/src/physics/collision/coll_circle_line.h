#ifndef CIRCLE_LINE_COLLISION
#define CIRCLE_LINE_COLLISION

#include "collision.h"
#include "solid_body/sb_circle.h"
#include "solid_body/sb_line.h"

class CircleLineCollision : public Collision {
public:
    CircleLineCollision(sb_Circle *first, sb_Line *second);

    static inline bool do_collide(sb_Circle *first, sb_Line *second) {
        Vec3d center_to_position = second->get_position() - first->get_position();
        Vec3d oriented_normal = second->get_normal().orient(center_to_position);
        oriented_normal.normalize();

        double distance = center_to_position.dot(oriented_normal);

        return first->get_radius() > distance;
    }

    void solve() override;

    static Collision *collide(sb_Circle *first, sb_Line *second);
};

#endif // CIRCLE_LINE_COLLISION