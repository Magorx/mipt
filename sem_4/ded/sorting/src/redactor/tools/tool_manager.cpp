#include "tool_manager.h"
#include "redactor/canvas/canvas.h"


ToolManager::ToolManager(Renderer *renderer):
Tool(renderer),
active_canvas(nullptr),
draw_color({0, 0, 0, 255}),
tools(),
active_tool(nullptr)
{}

ToolManager::~ToolManager() {
    active_canvas = nullptr;
    for (auto tool : tools) {
        // tool->on_activate();
        delete tool;
    }
}

void ToolManager::add_tool(Tool *tool, Hotkey hotkey) {
    if (!tool) {
        return;
    }

    if (hotkey != Hotkey::none) tool->set_hotkey(hotkey);
    tools.push_back(tool);
}

void ToolManager::set_draw_layer(Layer* layer) {
    draw_layer = layer;
    update_active_tool();
}

void ToolManager::set_draw_color(RGBA color) {
    draw_color = color;
    update_active_tool();
}

void ToolManager::set_active_tool(size_t idx) {
    if (idx >= tools.size()) {
        return;
    }

    Tool *tool = tools[idx];
    if (active_tool == tool) {
        return;
    }

    if (active_tool) active_tool->on_deactivate();

    active_tool = nullptr;

    if (tool == nullptr) {
        return;
    }

    active_tool = tool;
    update_active_tool();

    tool->on_activate();
}

void ToolManager::set_active_tool(Hotkey key) {
    if (!active_tool) {
        for (size_t i = 0; i < tools.size(); ++i) {
            Tool *tool = tools[i];
            if (tool->get_hotkey() == key) {
                set_active_tool(i);
                return;
            }
        }

        return;
    }

    int active_idx = get_active_tool_idx();
    for (int i = active_idx + 1; i < (int) tools.size(); ++i) {
        Tool *tool = tools[i];
        if (tool->get_hotkey() == key) {
            set_active_tool(i);
            return;
        }
    }

    for (int i = 0; i < active_idx; ++i) {
        Tool *tool = tools[i];
        if (tool->get_hotkey() == key) {
            set_active_tool(i);
        }
    }
}

int ToolManager::get_active_tool_idx() {
    if (!active_tool) return -1;

    for (size_t i = 0; i < tools.size(); ++i) {
        if (tools[i] == active_tool) {
            return i;
        }
    }

    return -1;
}

void ToolManager::set_active_canvas(Canvas *canvas) {
    active_canvas = canvas;
    update_active_tool();
}

void ToolManager::update_active_tool() {
    if (!active_tool) {
        return;
    }

    active_tool->set_draw_color(draw_color);
    active_tool->set_size(size);

    if (active_canvas) {
        Layer *layer_for_tool = nullptr;
        // if (active_canvas->get_draw_mode() == Canvas::DrawMode::use_active_layer) layer_for_tool = active_canvas->get_active_layer();
        // if (active_canvas->get_draw_mode() == Canvas::DrawMode::use_draw_layer)   layer_for_tool = active_canvas->get_draw_layer();
        layer_for_tool = active_canvas->get_draw_layer();
        active_tool->set_draw_layer(layer_for_tool);
    } else {
        active_tool->set_draw_layer(nullptr);
    }

    active_tool->on_update();
}

void ToolManager::on_mouse_down(const Vec2d &pos) {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_mouse_down(pos);
    }
}

void ToolManager::on_mouse_up(const Vec2d &pos) {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_mouse_up(pos);
    }
}

void ToolManager::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_mouse_move(from, to);
    }
}

void ToolManager::on_activate() {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_activate();
    }
}
void ToolManager::on_deactivate() {
    if (!active_tool) {
        return;
    } else {
        active_tool->on_deactivate();
    }
}
