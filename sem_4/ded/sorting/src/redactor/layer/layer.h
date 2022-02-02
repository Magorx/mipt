#pragma once


#include "visual/renderer/gate_type_aliases.h"
#include "visual/renderer/renderer.h"

#include "utils/affected.h"


class Canvas;


struct Layer : public Affected<Layer> {
    Renderer *renderer;
    Canvas   *canvas;

    RRendTexture *target;
    RRendTexture *final_target;
    Vec2d size;

    int idx;

    bool visible = true;

    RImage saved_image;
    bool saved_image_done = false;
    bool saved_image_needed = false;

    bool is_cleared = true;

    Layer(Renderer *renderer, Canvas *canvas, Vec2d size, int idx = 0);
    Layer(Renderer *renderer, const char *path);

    virtual ~Layer() {
        delete target;
        delete final_target;
    }

    inline void set_canvas(Canvas *canvas_) {
        canvas = canvas_;
    }

    inline Canvas* get_canvas() { return canvas; }

    void copy_from(const RTexture *img) {
        renderer->push_target(get_target());
        renderer->set_render_state(sf::BlendNone);
        renderer->draw_texture({0, 0}, img, true);
        renderer->pop_target();
    }

    void copy_from(const RImage *img) {
        RTexture texture;
        texture.create(img->getSize().x, img->getSize().y);
        texture.update(*img);
        copy_from(&texture);
    }

    void copy_from(const RGBA *data) {
        RTexture texture;
        texture.create(size.x(), size.y());
        texture.update((uint8_t*) data);
        copy_from(&texture);
    }

    void copy_from(const char *path) {
        RTexture texture;
        if (!texture.loadFromFile(path)) {
            logger.error("Layer", "can't open file [%s]", path);
            return;
        }
        copy_from(&texture);
    }

    Layer *copy() {
        Layer *ret = new Layer(renderer, nullptr, size);
        flush_to(ret);
        return ret;
    }

    RImage *copy_to_image() {
        auto img = new RImage;
        *img = target->getTexture().copyToImage();
        return img;
    }

    void fill_with(RTexture *img);
    
    void force_redraw();

    inline RRendTexture *get_target() {
        force_redraw();
        return target;
    }

    const RTexture *get_texture() {
        return &target->getTexture();
    }

    const RTexture *get_final_texture() {
        return &target->getTexture();
    }

    RColor get_pixel_color(const Vec2d &position);

    void clear(const RColor &color = {0, 0, 0, 0}) { target->clear(to_glib_color(color)); force_redraw(); is_cleared = true; }

    void flush_to(Layer *layer, bool to_flip = false, bool to_apply_effects = false, RMode rmode = {});
    void flush_to(Layer *layer, bool to_flip = false, bool to_apply_effects = false, RMode rmode = {}) const;

    void save_to(const char *filename) {
        target->getTexture().copyToImage().saveToFile(filename);
    }

    inline Vec2d flip(const Vec2d &p) { return {p.x(), size.y() - p.y()}; }

    inline bool is_ok() const {
        return size.x() > 0 && size.y() > 0;
    }

};


class FlushToFinalTexture : public Effect<Layer> {
public:
    FlushToFinalTexture(Layer *layer) : Effect(layer) {}

    virtual void apply() override {
        target->renderer->push_target(target->final_target);
        target->renderer->set_render_state(sf::BlendNone);
        target->renderer->draw_texture({0, 0}, target->get_texture(), true);
        target->renderer->pop_target();
    }
};


class ShaderEffect : public Effect<Layer> {
protected:
    RShader *shader;

public:
    ShaderEffect(Layer *layer, const char *filename) : Effect(layer), shader(new RShader) {
        shader->loadFromFile(filename, sf::Shader::Fragment);
        shader->setUniform("texture", sf::Shader::CurrentTexture);
    }

    virtual ~ShaderEffect() {
        delete shader;
    }

    virtual void apply() override {
        target->renderer->push_target(target->final_target);
        target->renderer->get_rstate()->rmode.blendMode = RBlend::none;
        target->renderer->get_rstate()->rmode.shader = shader;
        target->renderer->draw_texture({0, 0}, target->get_final_texture(), true);
        target->renderer->pop_target();
    }
};
