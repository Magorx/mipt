#pragma once


#include "appearence.h"


class AppearenceColor : public Appearence {
    sf::Color color;
public:
    AppearenceColor(const RColor &color): Appearence(), color(to_glib_color(color)) {}

    virtual RVertex vertex(Vec2d on_screen_position, Vec2d) const override {
        return {{(float) on_screen_position[0], (float) on_screen_position[1]}, color};
    };

    virtual RColor get_px_color(Vec2d) const override { return {color.r, color.g, color.b, color.a}; }

    void set_color(const RColor &color_) { color = to_glib_color(color_); }
};
