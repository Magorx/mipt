#include "target.h"
#include "plugin_shader.h"

#include "engine/Resources.h"
#include "engine/moga_engine.h"


Target::Target(Layer *layer, bool to_delete_layer, bool to_flip) :
layer(layer),
to_delete_layer(to_delete_layer),
to_flip(to_flip)
{}

Target::Target(Vec2d size) :
layer(nullptr)
{
    auto renderer = App.engine->visual->get_renderer();
    if (!renderer) return;
    
    layer = new Layer(renderer, nullptr, size);
    to_delete_layer = true;
    to_flip = true;
}

Target::Target(const char *path) :
layer(nullptr)
{
    auto renderer = App.engine->visual->get_renderer();
    if (!renderer) return;
    
    layer = new Layer(renderer, path);
    if (!layer || !layer->is_ok()) {
        delete layer;
        layer = nullptr;
        return;
    }
    to_delete_layer = true;
    to_flip = true;
}

Target::~Target() {
    if (to_delete_layer) delete layer;
}


PUPPY::RenderTarget *Target::get_copy() const {
    return new Target(layer->copy(), true);
}

PUPPY::Vec2s Target::get_size() const {
    return {(size_t) layer->size.x(), (size_t) layer->size.y()};
}

PUPPY::RGBA Target::Target::get_pixel(size_t x, size_t y) const {
    return (PUPPY::RGBA) layer->get_pixel_color({(double) x, (double) y}).ui32;
}

void Target::set_pixel(size_t x, size_t y, const PUPPY::RGBA &color) {
    auto img = layer->copy_to_image();
    RGBA rgba = color.ui32;
    img->setPixel(x, y, {rgba.r, rgba.g, rgba.b, rgba.a});
    layer->copy_from(img);
}

PUPPY::RGBA *Target::get_pixels() const {
    auto img = layer->copy_to_image();
    auto size = img->getSize();
    
    PUPPY::RGBA *data = new PUPPY::RGBA[size.x * size.y];
    memcpy(data, img->getPixelsPtr(), size.x * size.y * sizeof(PUPPY::RGBA));
    
    return data;
}

void Target::clear(const PUPPY::RGBA &color) {
    layer->clear(color.ui32);
}

#define INIT_DRAW_OBJECTS_       \
auto renderer = layer->renderer; \

#define PROCESS_RMODE_(mode) \
do { auto rstate = renderer->get_rstate(); \
if (mode.blend == PUPPY::BlendMode::COPY) rstate->rmode.blendMode = RBlend::none; \
if (mode.shader) {rstate->rmode.shader = (RShader*) mode.shader;} } while(0)

void Target::render_circle(const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) {
    INIT_DRAW_OBJECTS_;
    Vec2d pos = {position.x, position.y};

    if (to_flip) {
        pos = layer->flip(pos);
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_circle(pos, radius, color.ui32);
    renderer->pop_target();
}

void Target::render_line(const PUPPY::Vec2f &start, const PUPPY::Vec2f &end, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) {
    INIT_DRAW_OBJECTS_

    Vec2d from = {start.x, start.y};
    Vec2d to   = {end.x  , end.y  };

    if (to_flip) {
        from = layer->flip(from);
        to   = layer->flip(to  );
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_line(from, to, color.ui32);
    renderer->pop_target();
}

void Target::render_triangle(const PUPPY::Vec2f &p1_, const PUPPY::Vec2f &p2_, const PUPPY::Vec2f &p3_, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) {
    INIT_DRAW_OBJECTS_

    Vec2d p1 = {p1_.x, p1_.y};
    Vec2d p2 = {p2_.x, p2_.y};
    Vec2d p3 = {p3_.x, p3_.y};

    if (to_flip) {
        p1 = layer->flip(p1);
        p2 = layer->flip(p2);
        p3 = layer->flip(p3);
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_triangle(p1, p2, p3, color.ui32);
    renderer->pop_target();
}

void Target::render_rectangle(const PUPPY::Vec2f &p1_, const PUPPY::Vec2f &p2_, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) {
    INIT_DRAW_OBJECTS_

    Vec2d p1 = {p1_.x, p1_.y};
    Vec2d p2 = {p2_.x, p2_.y};

    if (to_flip) {
        p1 = layer->flip(p1);
        p2 = layer->flip(p2);
    }

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_rectangle(p1, p2 - p1, color.ui32);
    renderer->pop_target();
}


void Target::render_texture(const PUPPY::Vec2f &position, const RenderTarget *texture, const PUPPY::RenderMode &render_mode) {
    INIT_DRAW_OBJECTS_
    auto target = dynamic_cast<const Target*>(texture);
    auto to_draw = target->get_layer();

    bool to_flip = false;
    Vec2d pos = {position.x, position.y};
    if (this->to_flip ^ to_flip) {
        pos = layer->flip(pos);
        to_flip ^= true;
    }

    Vec2d shift = {(double) 0, - (double) target->get_size().y};
    
    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_texture(pos + shift, &to_draw->target->getTexture());
    renderer->pop_target();
}

void Target::render_pixels(const PUPPY::Vec2f &position, const PUPPY::Vec2s &size, const PUPPY::RGBA *data, const PUPPY::RenderMode &render_mode) {
    INIT_DRAW_OBJECTS_

    bool to_flip = false;
    Vec2d pos = {position.x, position.y};
    if (this->to_flip ^ to_flip) {
        pos = layer->flip(pos);
        to_flip ^= true;
    }

    RTexture texture;
    texture.create(size.x, size.y);
    texture.update((uint8_t*) data);

    Vec2d shift = {(double) 0, - (double) size.y};

    renderer->push_target(layer->get_target());
    PROCESS_RMODE_(render_mode);
    renderer->draw_texture(pos + shift, &texture, to_flip);
    renderer->pop_target();
}

void Target::apply_shader(const PUPPY::Shader *shader) {
    INIT_DRAW_OBJECTS_;

    auto *rshader = dynamic_cast<const PluginShader*>(shader)->get_shader();

    layer->get_target()->setRepeated(false);
    auto texture = &layer->get_target()->getTexture();

    RRendTexture new_tx;
    new_tx.create(texture->getSize().x, texture->getSize().y);
    renderer->push_target(&new_tx);
    renderer->get_rstate()->rmode.blendMode = RBlend::none;
    renderer->get_rstate()->rmode.texture   = texture;
    renderer->get_rstate()->rmode.shader    = rshader;
    renderer->draw_texture({0, 0}, texture, true);
    renderer->pop_target();

    renderer->push_target(layer->get_target());

    renderer->get_rstate()->rmode.blendMode = RBlend::none;
    renderer->get_rstate()->rmode.texture   = &new_tx.getTexture();
    renderer->draw_texture({0, 0}, &new_tx.getTexture(), true);
    renderer->pop_target();

    layer->get_target()->setRepeated(true);
}

void Target::update(const PUPPY::RGBA *data) {
    layer->copy_from((RGBA*) data);
}
