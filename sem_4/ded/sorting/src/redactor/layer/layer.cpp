#include "layer.h"
#include "redactor/canvas/canvas.h"
#include "visual/renderer/appearence.h"


Layer::Layer(Renderer *renderer, Canvas *canvas, Vec2d size, int idx):
Affected(this),
renderer(renderer),
canvas(canvas),
target(nullptr),
final_target(nullptr),
size(size),
idx(idx)
{
    target = new RRendTexture;
    target->create((float) size.x(), (float) size.y());
    target->setRepeated(true);
    target->clear({0, 0, 0, 0});

    final_target = new RRendTexture;
    final_target->create((float) size.x(), (float) size.y());
    final_target->setRepeated(true);
    final_target->clear({0, 0, 0, 0});

    add_effect(new FlushToFinalTexture(this));
}

Layer::Layer(Renderer *renderer, const char *path):
Affected(this),
renderer(renderer),
canvas(nullptr),
target(nullptr),
final_target(nullptr),
size(0),
idx(0)
{
    RTexture texture;
    if (!texture.loadFromFile(path)) {
        logger.error("Layer", "can't open file [%s]", path);
        return;
    }

    size = {(double) texture.getSize().x, (double) texture.getSize().y};

    target = new RRendTexture;
    target->create((float) size.x(), (float) size.y());
    target->setRepeated(true);
    target->clear({0, 0, 0, 0});

    final_target = new RRendTexture;
    final_target->create((float) size.x(), (float) size.y());
    final_target->setRepeated(true);
    final_target->clear({0, 0, 0, 0});

    // Layer l(renderer, nullptr, size);
    // l.copy_from(&texture);
    // l.flush_to(this);
    // target->getTexture().copyToImage().saveToFile("a.png");
    copy_from(&texture);
    

    add_effect(new FlushToFinalTexture(this));
}

void Layer::flush_to(Layer *layer, bool to_flip, bool to_apply_effects, RMode rmode) {
    if (to_apply_effects && !effects_applied) {
        apply_effects();
        effects_applied = true;
    }

    if (!saved_image_done && saved_image_needed) {
        saved_image_done = true;

        Layer inter(renderer, nullptr, size);
        inter.saved_image_needed = false;
        flush_to(&inter, false, true);
        saved_image = inter.target->getTexture().copyToImage();
    }

    if (!layer || !renderer) return;

    renderer->push_target(layer->get_target());
    renderer->set_render_state(rmode);

    if (to_apply_effects) {
        renderer->draw_texture({0, 0}, &final_target->getTexture(), to_flip);
    } else {
        renderer->draw_texture({0, 0}, &target->getTexture(), to_flip);
    }

    renderer->pop_target();
}

void Layer::flush_to(Layer *layer, bool to_flip, bool to_apply_effects, RMode rmode) const {
    if (!layer || !renderer) return;

    renderer->push_target(layer->get_target());
    renderer->set_render_state(rmode);

    if (to_apply_effects) {
        renderer->draw_texture({0, 0}, &final_target->getTexture(), to_flip);
    } else {
        renderer->draw_texture({0, 0}, &target->getTexture(), to_flip);
    }

    renderer->pop_target();
}

void Layer::force_redraw() {
    effects_applied = false;
    saved_image_done = false;
    is_cleared = false;
    if (canvas) canvas->force_redraw();
}

void Layer::fill_with(RTexture *img) {
    if (!img) return;

    AppearenceTexture appr(img);

    renderer->push_target(get_target());
    renderer->set_appearence(&appr);
    renderer->apr_draw_rectangle({0, 0}, size);
    renderer->pop_target();
}

RColor Layer::get_pixel_color(const Vec2d &position) {
    flush_to(nullptr, false, true);
    auto color = Renderer::get_pixel_color(position, &final_target->getTexture());

    return color;
}
