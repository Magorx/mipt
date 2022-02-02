#ifndef RENDERER_H
#define RENDERER_H


#include <SFML/Graphics.hpp>
#include <cstring>

#include "utils/vec3d.h"
#include "utils/vec2d.h"
#include "visual/color/color.h"

#include "appearence/appearence.h"

#include "engine/Resources.h"


struct Screen {
    sf::RenderWindow *window;

    inline Screen(sf::RenderWindow *window):
    window(window)
    {}
};


struct RendererState {
    Vec2d offset;
    sf::RenderTarget *target;

    Appearence *appearence;
    RState rmode;

    // RendererState(Vec2d offset, sf::RenderTarget *target, Appearence *appearence, sf::RenderStates sf_state) :
    // offset(offset),
    // target(target),
    // appearence(appearence),
    // sf_state(sf_state)
    // {}
};


class Renderer {
    Screen scr;

    std::vector<RVertex> cur_verticies;

    std::vector<RendererState> states;
    RendererState *state;

public:
    Renderer(RWindow *window);
    ~Renderer();

    inline void clear(sf::Color color = sf::Color(0, 0, 0, 255)) { scr.window->clear(color); }
    inline void display() { scr.window->display(); }

    inline sf::RenderWindow *get_window() const {
        return scr.window;
    }

    inline void shift(const Vec2d &delta) {
        state->offset += delta;
    }

    void draw_circle(Vec2d pos, const double rad, const RGBA &color);
    void draw_line(Vec2d p1, Vec2d p2, const RGBA &color);
    void draw_square(Vec2d pos, const double size, const RGBA &color);
    void draw_triangle(Vec2d p1, Vec2d p2, Vec2d p3, const RGBA &color);
    void draw_rectangle(Vec2d pos, const Vec2d size, const RGBA &color, const RGBA &bounds_color = {0, 0, 0, 0});
    void draw_text(const char *label, int size, Vec2d pos, const RGBA &back_color, const RGBA &font_color,  bool to_background, bool to_centrize = false, const RFont *font = App.font.basic);

    void apr_draw_circle(Vec2d pos, double rad, int granularity);
    void apr_draw_rectangle(Vec2d pos, const Vec2d size);

    void draw_texture(Vec2d pos, const RTexture *texture, bool to_flip = true);

    inline void set_appearence(Appearence *appearence_) { state->appearence = appearence_; }
    inline void set_render_state(RState rstate) { state->rmode = rstate; }

    static Vec2d get_text_size(const char *text, int char_size, const RFont *font = App.font.basic);
    static Vec2d get_char_position(const char *text, int idx, int char_size, const RFont *font = App.font.basic);
    static RGBA get_pixel_color(Vec2d pos, const RTexture *texture);

    void push_target(sf::RenderTarget *target) {
        if (state) {
            states.push_back({{0, 0}, target, state->appearence, state->rmode});
        } else {
            states.push_back({{0, 0}, target, nullptr, {sf::BlendAlpha}});
        }

        state = &states[states.size() - 1];
    }

    void pop_target() {
        if (states.size() <= 1) {
            return;
        }

        states.pop_back();
        state = &states[states.size() - 1];
    }

    RendererState *get_rstate() { return state; }
};

#endif // RENDERER_H