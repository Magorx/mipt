#pragma once


#include "vertical_layout.h"
#include "button.h"
#include "visual/renderer/renderer.h"

#include <vector>


class v_Menu : public v_VerticalLayout {
    std::vector<char*> labels;

    // SmartColor *button_color;
    // SmartColor *text_color;
    
    int char_size;
    Vec2d button_scale;

    Vec2d min_size;

public:
    v_Menu(const std::vector<const char*> &button_labels, SmartColor *button_color, RGBA text_color, SmartColor *back_color = nullptr, int char_size = 15, Vec2d button_scale = {1.3, 1.3});
    virtual ~v_Menu();

    AbstractView *operator[](size_t i);

    virtual void refit() override;

    void recalculate_size();
    
    inline void set_min_size(Vec2d min_size_) { min_size = min_size_; }
};
