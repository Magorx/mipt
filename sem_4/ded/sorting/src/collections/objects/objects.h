#ifndef OBJECTS_COLLECTION_H
#define OBJECTS_COLLECTION_H

#include "engine/object.h"

#include "../Collections.h"

class o_Ball : public Object {
public:
    o_Ball(Vec3d pos, double rad, SmartColor *color, double mass=1);

    virtual void tick(const double = 0, const double = 0) override;
};

class o_Rect : public Object {
public:
    o_Rect(Vec3d pos, double rad, SmartColor *color, double mass=1);

    virtual void tick(const double = 0, const double = 0) override;
};

class o_Line : public Object {
public:
    o_Line(Vec3d p1, Vec3d p2, SmartColor *color);

    virtual void tick(const double = 0, const double = 0) override;
};

#endif // OBJECTS_COLLECTION_H