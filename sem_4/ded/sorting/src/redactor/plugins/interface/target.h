#pragma once


#include "redactor/plugin_std/plugin_std.hpp"
#include "redactor/layer/layer.h"


class Target : public PUPPY::RenderTarget {
    Layer *layer;
    bool to_delete_layer = false;
    bool to_flip = false;

public:
    Target(Layer *layer, bool to_delete_layer = false, bool to_flip = false);
    Target(Vec2d size);
    Target(const char *path);
    virtual ~Target();

    const Layer *get_layer() const { return layer; }
    Layer *get_layer() { return layer; }
    void update(const PUPPY::RGBA *data);

    virtual PUPPY::RenderTarget *get_copy() const override;

    virtual PUPPY::Vec2s get_size() const override;

    virtual PUPPY::RGBA get_pixel(size_t x, size_t y)          const override;
    virtual void    set_pixel(size_t x, size_t y, const PUPPY::RGBA &color) override;

    virtual PUPPY::RGBA *get_pixels() const override;

    virtual void clear(const PUPPY::RGBA &color = 0) override;

// render
    virtual void render_circle      (const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode)       override;
    virtual void render_line        (const PUPPY::Vec2f &start, const PUPPY::Vec2f &end, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode)          override;
    virtual void render_triangle    (const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::Vec2f &p3, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode) override;
    virtual void render_rectangle   (const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode)              override;
    
    virtual void render_texture     (const PUPPY::Vec2f &position, const RenderTarget *texture, const PUPPY::RenderMode &render_mode) override;
    virtual void render_pixels      (const PUPPY::Vec2f &position, const PUPPY::Vec2s &size, const PUPPY::RGBA *data, const PUPPY::RenderMode &render_mode) override;

    virtual void apply_shader(const PUPPY::Shader *shader) override;

    bool is_ok() const {
        return layer;
    }
};
