#include "color_pipett.h"
#include "redactor/canvas/canvas.h"


t_ColorPipett::t_ColorPipett(Tool *manager) :
Tool(manager, "[q] Pipete")
{
    if (!manager) {
        logger.warning("ColorPipette", "nullptr manager, luckily to crush now");
    }
}

void t_ColorPipett::on_mouse_down(const Vec2d &pos) {
    Tool::on_mouse_down(pos);

    auto color = draw_layer->get_pixel_color(pos);
    tool_manager->set_draw_color(color);
}

void t_ColorPipett::on_mouse_up(const Vec2d &pos) {
    Tool::on_mouse_up(pos);
}

void t_ColorPipett::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    Tool::on_mouse_move(from, to);

    if (mouse_down) tool_manager->set_draw_color(draw_layer->get_pixel_color(to));
}

void t_ColorPipett::on_activate() {
    if (draw_layer) draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_active_layer);
}

void t_ColorPipett::on_update() {
    if (draw_layer) {
        set_draw_layer(draw_layer->get_canvas()->get_active_layer());
        draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_active_layer);
    }
}

