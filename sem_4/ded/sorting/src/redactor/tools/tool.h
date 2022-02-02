#pragma once

#include <cstdio>

#include "redactor/layer/layer.h"

#include "visual/renderer/renderer.h"

#include "device/keyboard.h"


class Tool {
protected:
    Renderer *renderer;
    Tool *tool_manager;

    Layer *draw_layer;
    RColor draw_color;

    bool mouse_down = false;
    Vec2d cur_pos = {0, 0};

    double size = 1;

    Hotkey hotkey = Hotkey::none;
    const char *name = nullptr;

public:
    Tool(Tool *manager, const char *name = nullptr) :
    renderer(manager ? manager->get_renderer() : nullptr),
    tool_manager(manager),
    draw_layer(nullptr),
    draw_color(0, 0, 0, 255),
    name(name)
    {}

    Tool(Renderer *renderer) :
    renderer(renderer),
    tool_manager(nullptr),
    draw_layer(nullptr),
    draw_color(0, 0, 0, 255)
    {}

    virtual ~Tool() {};

    inline void set_hotkey(Hotkey key) { hotkey = key; }
    inline Hotkey get_hotkey() { return hotkey; }

    virtual void set_draw_layer(Layer* layer) { draw_layer = layer; }
    virtual void set_draw_color(RColor color) { draw_color = color; }
    void set_renderer(Renderer *renderer_) { renderer = renderer_; }

    virtual void on_mouse_down(const Vec2d &pos) {
        mouse_down = true;
        cur_pos = pos;
    }

    virtual void on_mouse_up(const Vec2d &pos) {
        mouse_down = false;
        cur_pos = pos;
    }

    virtual void on_mouse_move(const Vec2d &, const Vec2d &to) {
        cur_pos = to;
    }
    
    virtual void on_activate() {}
    virtual void on_deactivate() {}
    virtual void on_update() {}

    virtual void set_size(double size_) {
        size = size_;
        size = size < 0 ? 0 : size;
    }

    virtual void shift_size(double delta) {
        set_size(size + delta);
    }

    double get_size() {
        return size;
    }

    inline Renderer *get_renderer() { return renderer; }

    const char *get_name() { return name; }

    virtual void open_settings() {}
};
