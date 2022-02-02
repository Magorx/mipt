#include "smart_color.h"

SmartColor::SmartColor() :
color(0, 0, 0)
{}

SmartColor::SmartColor(const Color &color_) :
color(color_)
{}

SmartColor::SmartColor(const RGBA  &rgba):
color(rgba.r, rgba.g, rgba.b, rgba.a)
{}

void SmartColor::set_rgb(const Color color_) {
    color = color_;
}

void SmartColor::set_rgb(const double r, const double g, const double b) {
    color.set(0, r);
    color.set(1, g);
    color.set(2, b);
}

const Color SmartColor::rgb() {
    return color;
}

void SmartColor::tick(const double, const double) {}
