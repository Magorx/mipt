#pragma once


#include "style.h"
#include "mouse_reaction_style.h"
#include "utility_bar_style.h"


struct WindowStyle : public Style {
    UtilityBarStyle *header;
    Appearence *body;

    WindowStyle(UtilityBarStyle *header, Appearence *body) :
    header(header),
    body(body)
    {}

    virtual ~WindowStyle() {
        delete header;
        delete body;
    }

};
