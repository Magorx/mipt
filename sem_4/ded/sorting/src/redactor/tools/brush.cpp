#include "brush.h"
#include "tool_manager.h"
#include "redactor/canvas/canvas.h"


 void t_Brush::draw_point(const Vec2d &pos) {
    if (!draw_layer || !renderer) {
        return;
    }

    auto target = draw_layer->get_target();

    renderer->push_target(target);

    renderer->set_render_state({sf::BlendNone});

    renderer->draw_circle(pos, size, draw_color);

    renderer->pop_target();
}

t_Brush::t_Brush(Tool *manager) :
Tool(manager, "[b] Bruh")
{}

void t_Brush::on_mouse_down(const Vec2d &pos) {
    Tool::on_mouse_down(pos);

    draw_point(pos);
}

void t_Brush::on_mouse_up(const Vec2d &pos) {
    Tool::on_mouse_up(pos);
}

void t_Brush::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    Tool::on_mouse_move(from, to);

    if (!mouse_down) return;

    Vec2d dir = to - from;
    dir.normalize();
    Vec2d p = from;

    while ((from - p).len_squared() < (from - to).len_squared()) {
        draw_point(p);
        p += dir;
    }

    draw_point(from);
    draw_point(to);
}

void t_Brush::on_activate() {
    if (draw_layer) draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_draw_layer);
}

void t_Brush::on_deactivate() {
    mouse_down = false;

    if (draw_layer) {
       if (Canvas *canvas = draw_layer->get_canvas()) {
           canvas->get_draw_layer()->clear();
       }
    }
}

void t_Brush::on_update() {
    if (draw_layer) draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_draw_layer);
}
