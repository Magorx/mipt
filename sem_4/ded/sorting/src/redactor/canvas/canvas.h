#pragma once


#include <vector>

#include "redactor/layer/layer.h"
#include "redactor/tools/tool_manager.h"

#include "canvas_history.h"


class Canvas {
public:
    enum DrawMode {
        use_draw_layer,
        use_active_layer,
    };

private:
    Renderer *renderer;
    ToolManager *tool_manager;
    Vec2d size;

    History history;

    std::vector<Layer*> layers;
    Layer *active_layer;

    Layer *prev_active_layer; // for history

    Layer *draw_layer;
    Layer *inter_action_layer;
    Layer *final_layer;
    Layer *transparency_squares_layer;

    bool _to_redraw;

    DrawMode draw_mode = DrawMode::use_draw_layer;

    void set_active_layer(Layer *layer);

    bool mouse_down = false;

public:
    Canvas(Renderer *renderer, ToolManager *tool_manager, Vec2d size);

    ~Canvas();

    Layer *get_active_layer() { return active_layer; }
    Layer *get_final_layer()  { return final_layer; }
    Layer *get_draw_layer()   { return draw_layer; }

    inline void force_redraw() { _to_redraw = true; }
    inline bool to_redraw()    { return _to_redraw; }

    inline void set_draw_mode(DrawMode draw_mode_) { draw_mode = draw_mode_; }
    inline DrawMode get_draw_mode() const { return draw_mode; }

    void flush_draw_to_active(bool to_copy = false);
    void flush_to_final();

    int new_layer();
    int next_layer(int delta);
    int next_layer();
    int prev_layer();
    int set_active_layer(int idx);

    void upd_prev_active_layer() {
        if (!active_layer) return;
        active_layer->flush_to(prev_active_layer, true, false, RBlend::none);
    }

    int idx_by_layer(Layer *layer);

    void grab_tool_manager_activity();

    void on_mouse_down(const Vec2d &pos);
    void on_mouse_up(const Vec2d &pos);
    void on_mouse_move(const Vec2d &from, const Vec2d &to);

    void save_to_file(const char *filename);

    void undo();
    void redo();
    void push_history();

    inline Vec2d flip(const Vec2d &p) { return {p.x(), size.y() - p.y()}; }
};
