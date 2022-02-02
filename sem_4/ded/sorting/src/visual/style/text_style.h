#pragma once


#include "style.h"
#include "visual/renderer/gate_type_aliases.h"


struct TextStyle : public Style {
    int size;
    RFont *font;
    RGBA foreground;
    RGBA background;

    TextStyle(int size, RFont *font, RGBA foreground, RGBA background) :
    size(size),
    font(font),
    foreground(foreground),
    background(background)
    {}

    TextStyle *negative() {
        foreground = !foreground;
        background = !background;

        return this;
    }

    virtual ~TextStyle() {}
};
