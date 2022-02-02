#ifndef COLLISION_H
#define COLLISION_H

class SolidBody;

struct Collision {
    SolidBody *first;
    SolidBody *second;

    Collision(SolidBody *first, SolidBody *second) :
    first(first),
    second(second)
    {}

    virtual ~Collision() {}

    virtual void solve() = 0;
};

#endif // COLLISION_H