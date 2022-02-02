#pragma once


#include "style.h"
#include "mouse_reaction_style.h"


struct UtilityBarStyle : public Style {
    MouseReactionStyle *close_button;
    MouseReactionStyle *hide_button;

    MouseReactionStyle *accessory;

    Appearence *l_corner;
    Appearence *r_corner;

    Appearence *bar;
    Appearence *underbar;

    UtilityBarStyle(MouseReactionStyle *close_button, MouseReactionStyle *hide_button, MouseReactionStyle *accessory, Appearence *bar, Appearence *l_corner, Appearence *r_corner, Appearence *underbar = nullptr) :
    close_button(close_button),
    hide_button(hide_button),
    accessory(accessory),
    l_corner(l_corner),
    r_corner(r_corner),
    bar(bar),
    underbar(underbar)
    {}

    virtual ~UtilityBarStyle() {
        delete close_button;
        delete hide_button;
        delete accessory;
        delete bar;
        delete underbar;

        delete l_corner;
        delete r_corner;
    }
};
