#pragma once


#include "menu.h"


class v_Submenu : public v_Highlighter {
    v_Menu *menu;
    ViewBody align;

public:
    enum Type {
        on_click,
        on_hover,
    };

    v_Submenu(Type type, const ViewBody &body, SmartColor *color, const char *label, std::vector<const char*> button_labels, SmartColor *button_color, RGBA text_color, SmartColor *back_color = nullptr, int char_size = 15, Vec2d button_scale = {1.3, 1.3});

    static inline v_Submenu *Button(const ViewBody &body, SmartColor *color, const char *label, std::vector<const char*> button_labels, SmartColor *button_color, RGBA text_color, SmartColor *back_color = nullptr, int char_size = 15, Vec2d button_scale = {1.3, 1.3}) {
        return new v_Submenu(Type::on_click, body, color, label, button_labels, button_color, text_color, back_color, char_size, button_scale);
    }

    static inline v_Submenu *Hover(const ViewBody &body, SmartColor *color, const char *label, std::vector<const char*> button_labels, SmartColor *button_color, RGBA text_color, SmartColor *back_color = nullptr, int char_size = 15, Vec2d button_scale = {1.3, 1.3}) {
        return new v_Submenu(Type::on_hover, body, color, label, button_labels, button_color, text_color, back_color, char_size, button_scale);
    }

    virtual void refit() override;

    AbstractView *operator[](size_t i);

    void set_align(const ViewBody &align_) { align = align_; refit(); }
    inline void set_min_size(Vec2d min_size) { menu->set_min_size(min_size); }
};
