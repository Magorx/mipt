#pragma once 


#include "style.h"
#include "visual/renderer/appearence/appearence.h"


struct MouseReactionStyle : public Style {
    Appearence *idle;

    Appearence *press;
    Appearence *release;

    Appearence *hover;
    Appearence *unhover;

    Appearence *move;

    MouseReactionStyle(Appearence *idle = nullptr, Appearence *press = nullptr, Appearence *release = nullptr, Appearence *hover = nullptr, Appearence *unhover = nullptr, Appearence *move = nullptr) :
    idle(idle),
    press(press),
    release(release),
    hover(hover),
    unhover(unhover),
    move(move)
    {}

    virtual ~MouseReactionStyle() {
        delete idle;
        delete release;
        delete hover;
        delete unhover;
        delete move;
    }
};
