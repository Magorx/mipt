#pragma once


#include "tool.h"

#include "visual/renderer/renderer.h"

#include "device/keyboard.h"

class Canvas;

class ToolManager : public Tool {
    Renderer *renderer;

    Canvas *active_canvas;
    RGBA draw_color;

    std::vector<Tool*> tools;
    Tool *active_tool;

public:

    ToolManager(Renderer *renderer);
    ~ToolManager();

    void add_tool(Tool *tool, Hotkey hotkey = Hotkey::none);
    void set_active_tool(size_t idx);
    void set_active_tool(Hotkey key);
    int get_active_tool_idx();
    void update_active_tool();
    inline Tool *get_active_tool() { return active_tool; }

    void set_active_canvas(Canvas *canvas);
    Canvas *get_active_canvas() { return active_canvas; }

    virtual void set_draw_layer(Layer* layer) override;
    virtual void set_draw_color(RGBA color) override;
    void set_renderer(Renderer *renderer_) { renderer = renderer_; }

    RGBA get_draw_color() const { return draw_color; }

    virtual void on_mouse_down(const Vec2d &pos) override;

    virtual void on_mouse_up(const Vec2d &pos) override;

    virtual void on_mouse_move(const Vec2d &from, const Vec2d &to) override;
    
    virtual void on_activate() override;
    virtual void on_deactivate() override;

    std::vector<Tool*> get_tools() { return tools; }
};
