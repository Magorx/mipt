#pragma once


#include "brush.h"
#include "redactor/canvas/canvas.h"


class t_Eraser : public t_Brush {
public:
    t_Eraser(Tool *manager) : t_Brush(manager) {
        name = "[e] Eraer";
    }

    virtual void on_activate() override {
        if (draw_layer) draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_active_layer);
    }

    virtual void on_update() override {
        if (draw_layer) {
            set_draw_layer(draw_layer->get_canvas()->get_active_layer());
            draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_active_layer);
        }
    }
};
