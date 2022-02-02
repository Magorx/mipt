#include "plugin_tool.h"
#include "redactor/canvas/canvas.h"


t_Plugin::t_Plugin(Tool *manager, RedactorPlugin *plugin) :
Tool(manager, plugin ? plugin->get_lib()->name : nullptr),
plugin(plugin)
{}

void t_Plugin::on_mouse_down(const Vec2d &pos) {
    plugin->on_mouse_press(pos);
}

void t_Plugin::on_mouse_up(const Vec2d &pos) {
    plugin->on_mouse_release(pos);
}

void t_Plugin::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    plugin->on_mouse_move(from, to);
}

void t_Plugin::on_activate() {
    if (draw_layer) draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_draw_layer);
}

void t_Plugin::on_update() {
    if (draw_layer) {
        draw_layer->get_canvas()->set_draw_mode(Canvas::DrawMode::use_draw_layer);
    }
}

void t_Plugin::open_settings() {
    plugin->show_settings();
    // if (!settings) return;

    // settings->e_toggle_activity.emit({});
}

