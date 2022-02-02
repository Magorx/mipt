#pragma once


#include "utils/vec2d.h"

template <typename T>
class Lambda {
public:
    virtual void operator()(const T& event) = 0;
};

class MouseLambda {
public:
    virtual void operator()(const Vec2d &pos) = 0;
};
