#pragma once


#error lol slider doesn't work yet, i'm lazy


#include "view/view.h"
#include "../renderables/renderables.h"


struct SliderRange {
    float min_val;
    float max_val;
    float cur_val;
};

template <typename T>
class v_Slider : public AbstractView {
    SliderRange range;
    T &val;
    SmartColor *bar_color;
    SmartColor *point_color;

    T (law)(const T &value, float factor);

public:
    v_Slider(const ViewBody &body, const SliderRange &range, T (law)(const T &value, float factor), 
             SmartColor *bar_color, SmartColor *point_color):
    body(body),
    range(range),
    law(law),
    bar_color(bar_color),
    point_color(point_color)
    {}
    

    virtual void tick(const double = 0, const double = 0) override;

    void bind(MouseLambda *on_click);

    virtual void clicked(Vec2d click) override;
    virtual void hovered(Vec2d from, Vec2d to) override;
    virtual void released(Vec2d click) override;
};
