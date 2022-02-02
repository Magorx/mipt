#include "canvas.h"


Canvas::Canvas(Renderer *renderer, ToolManager *tool_manager, Vec2d size):
renderer(renderer),
tool_manager(tool_manager),
size(size),
history(25)
{
    if (!renderer || !tool_manager) {
        printf("Canvas was created without renderer or tool_manager, luckily to crush now\n");
    }

    active_layer = new Layer(renderer, this, size);
    layers.push_back(active_layer);

    prev_active_layer = new Layer(renderer, this, size);
    set_active_layer(active_layer);

    draw_layer = new Layer(renderer, this, size);
    final_layer = new Layer(renderer, this, size);
    inter_action_layer = new Layer(renderer, this, size);
    transparency_squares_layer = new Layer(renderer, this, size);

    transparency_squares_layer->fill_with(App.texture.transparency_squares);
}

Canvas::~Canvas() {
    for (auto layer : layers) {
        delete layer;
    }

    delete prev_active_layer;
    delete draw_layer;
    delete inter_action_layer;
    delete transparency_squares_layer;
}

void Canvas::set_active_layer(Layer *layer) {
    active_layer = layer;
    upd_prev_active_layer();
}

void Canvas::push_history() {
    history.add(new CanvasHistoryState(this, idx_by_layer(active_layer), prev_active_layer, active_layer));
    set_active_layer(active_layer);
}

void Canvas::flush_draw_to_active(bool to_copy) {
    if (to_copy) {
        draw_layer->flush_to(inter_action_layer, true, false, RBlend::none);
        inter_action_layer->flush_to(active_layer, false, false, RBlend::none);
        draw_layer->clear({0, 0, 0, 0});
        return;
    }

    if (draw_mode == DrawMode::use_draw_layer) {
        active_layer->flush_to(inter_action_layer, false, false, RBlend::none);
        draw_layer->flush_to(inter_action_layer, true, false);
        inter_action_layer->flush_to(active_layer, false, false, RBlend::none);
        draw_layer->clear({0, 0, 0, 0});
    }

    if (!mouse_down) return;

    push_history();
}

void Canvas::flush_to_final() {
    final_layer->clear();
    transparency_squares_layer->flush_to(final_layer);
    for (auto layer : layers) {
        if (layer == active_layer) {
            inter_action_layer->clear();
            active_layer->flush_to(inter_action_layer, false, false, RBlend::none);
            draw_layer->flush_to(inter_action_layer, true, false);

            inter_action_layer->set_effects(active_layer->get_effects());
            inter_action_layer->flush_to(final_layer, false, true);
        } else {
            layer->flush_to(final_layer, true, true);
        }
    }

    _to_redraw = false;
}

int Canvas::new_layer() {
    Layer *new_layer = new Layer(renderer, this, size);
    layers.push_back(new_layer);

    set_active_layer(new_layer);

    flush_to_final();
    return layers.size() - 1;
}

int Canvas::next_layer(int delta) {
    if (!layers.size()) return -1;
    if (!active_layer) {
        set_active_layer(layers[0]);
        flush_to_final();
        return 0;
    }
    
    int cur_idx = idx_by_layer(active_layer);
    if (cur_idx == -1) {
        return -1;
    }

    if (cur_idx + delta < 0 || cur_idx + delta >= (int) layers.size()) {
        return cur_idx;
    }

    set_active_layer(layers[cur_idx + delta]);
    flush_to_final();
    return cur_idx + delta;
}

int Canvas::next_layer() {
    return next_layer(+1);
}

int Canvas::prev_layer() {
    return next_layer(-1);
}

int Canvas::set_active_layer(int idx) {
    if (idx < 0 || idx >= (int) layers.size()) {
        return -1;
    }

    set_active_layer(layers[idx]);
    return idx;
}

int Canvas::idx_by_layer(Layer *layer) {
    for (size_t i = 0; i < layers.size(); ++i) {
        if (layer == layers[i]) {
            return i;
        }
    }

    return -1;
}

void Canvas::grab_tool_manager_activity() { tool_manager->set_active_canvas(this); }

void Canvas::on_mouse_down(const Vec2d &pos) {
    mouse_down = true;

    if (draw_mode == DrawMode::use_active_layer) {
        tool_manager->on_mouse_down(flip(pos));
    } else {
        tool_manager->on_mouse_down(pos);
    }
}

void Canvas::on_mouse_up(const Vec2d &pos) {
    if (!mouse_down) return;

    if (draw_mode == DrawMode::use_active_layer) {
        tool_manager->on_mouse_up(flip(pos));
    } else {
        tool_manager->on_mouse_up(pos);
    }

    flush_draw_to_active();
    flush_to_final();

    mouse_down = false;
}

void Canvas::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    if (draw_mode == DrawMode::use_active_layer) {
        tool_manager->on_mouse_move(flip(from), flip(to));
    } else {
        tool_manager->on_mouse_move(from, to);
    }
}

void Canvas::save_to_file(const char *filename) {
    if (!filename) {
        return;
    }

    int len = strlen(filename);
    char *name = (char*) calloc(len + 4, sizeof(char));
    strcpy(name, filename);
    name[len + 0] = '.';
    name[len + 1] = 'p';
    name[len + 2] = 'n';
    name[len + 3] = 'g';

    flush_to_final();
    final_layer->get_texture()->copyToImage().saveToFile(name);

    free(name);
}

void Canvas::undo() {
    history.undo();
}

void Canvas::redo() {
    history.redo();
}
