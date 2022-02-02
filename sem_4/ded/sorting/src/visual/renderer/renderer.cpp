#include "renderer.h"


Renderer::Renderer(RWindow *window):
scr(window),
state(nullptr)
{
    push_target(scr.window);
    scr.window->setFramerateLimit(60);
}

Renderer::~Renderer() {
    delete get_window();
}

void Renderer::draw_circle(Vec2d pos, const double rad, const RGBA &color) {
    pos += state->offset;

    sf::CircleShape circle(rad);

    circle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    circle.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    circle.setOrigin({static_cast<float>(rad), static_cast<float>(rad)});

    state->target->draw(circle, state->rmode);
}

void Renderer::draw_line(Vec2d p1, Vec2d p2, const RGBA &color) {
    p1 += state->offset;
    p2 += state->offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);

    sf::Vertex buffer[2] = {
        sf::Vertex({static_cast<float>(p1.x()), static_cast<float>(p1.y())}, sf_color),
        sf::Vertex({static_cast<float>(p2.x()), static_cast<float>(p2.y())}, sf_color),
    };

    state->target->draw(buffer, 2, sf::Lines, state->rmode);
}

void Renderer::draw_square(Vec2d pos, const double size, const RGBA &color) {
    pos += state->offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);

    sf::RectangleShape rect({static_cast<float>(2 * size), static_cast<float>(2 * size)});
    rect.setOrigin({static_cast<float>(size), static_cast<float>(size)});
    rect.setPosition({static_cast<float>(pos.x()), static_cast<float>(pos.y())});
    rect.setFillColor(sf_color);

    state->target->draw(rect, state->rmode);
}

void Renderer::draw_triangle(Vec2d p1, Vec2d p2, Vec2d p3, const RGBA &color) {
    p1 += state->offset;
    p2 += state->offset;
    p3 += state->offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);

    sf::Vertex buffer[3] = {
        sf::Vertex({static_cast<float>(p1.x()), static_cast<float>(p1.y())}, sf_color),
        sf::Vertex({static_cast<float>(p2.x()), static_cast<float>(p2.y())}, sf_color),
        sf::Vertex({static_cast<float>(p3.x()), static_cast<float>(p3.y())}, sf_color),
    };

    state->target->draw(buffer, 3, sf::Triangles, state->rmode);
}

void Renderer::draw_rectangle(Vec2d pos, const Vec2d size, const RGBA &color, const RGBA &bounds_color) {
    pos += state->offset;

    sf::Color sf_color(color.r, color.g, color.b, color.a);
    sf::Color sf_bounds_color(bounds_color.r, bounds_color.g, bounds_color.b, bounds_color.a);

    sf::RectangleShape rect({static_cast<float>(size.x()), static_cast<float>(size.y())});
    rect.setPosition({static_cast<float>(-size.x() / 2), static_cast<float>(-size.y() / 2)});
    rect.setOrigin({static_cast<float>(-pos.x() - size.x() / 2), static_cast<float>(-pos.y() - size.y() / 2)});
    rect.setFillColor(sf_color);

    rect.setOutlineColor(sf_bounds_color);
    rect.setOutlineThickness(1);

    state->target->draw(rect, state->rmode);
}

void Renderer::draw_text(const char *label, int char_size, Vec2d pos, const RGBA &font_color, const RGBA &back_color, bool to_background, bool to_centrize, const RFont *font) {
    pos += (Vec2d) state->offset;

    sf::Color sf_color_back(back_color.r, back_color.g, back_color.b, back_color.a);
    sf::Color sf_color_font(font_color.r, font_color.g, font_color.b, font_color.a);

    sf::Text text;

    text.setFont(*font);
    text.setString(label);
    text.setCharacterSize(char_size);

    text.setFillColor(sf_color_font);
    // text.setOutlineColor(sf_color_font);
    // text.setOutlineThickness(1);

    sf::Vector2f sf_pos(static_cast<float>(pos.x()), static_cast<float>(pos.y()));
    // sf::Vector2f sf_pos(static_cast<float>((int) pos.x()), static_cast<float>((int) pos.y()));

    text.setPosition(sf_pos);
    sf::Vector2f bounds(text.getLocalBounds().width, text.getLocalBounds().height);

    if (to_centrize) {
        text.setPosition(text.getPosition() - sf::Vector2f{bounds.x / 2, (float) ((float) char_size / 1.4)});
    }

    text.setPosition({(float) (int) text.getPosition().x, (float) (int) text.getPosition().y});

    if (to_background) {
        bounds.y *= 2;
        sf::RectangleShape background(bounds);
        background.setFillColor(sf_color_back);
        background.setPosition(text.getPosition());
        state->target->draw(background, state->rmode);
    }

    state->target->draw(text, state->rmode);
}

void Renderer::draw_texture(Vec2d pos, const RTexture *texture, bool to_flip) {
    if (!texture) return;

    pos += state->offset;

    // texture->setRepeated(true);

    sf::Sprite sprite(*texture);
    sprite.setPosition({(float) pos.x(), (float) pos.y()});
    // sprite.setColor({255, 255, 255, 255});

    if (to_flip) {
        auto size = sprite.getLocalBounds();

        sprite.setOrigin(0, size.height);
        sprite.setScale(1, -1);
    } 

    state->target->draw(sprite, state->rmode);
}

Vec2d Renderer::get_text_size(const char *text, int char_size, const RFont *font) {
    sf::Text sf_text = {};
    sf_text.setFont(*font);
    sf_text.setString(text);
    sf_text.setCharacterSize(char_size);

    sf::Vector2f bounds(sf_text.getLocalBounds().width, sf_text.getLocalBounds().height);
    return {bounds.x, bounds.y * 2};
}

Vec2d Renderer::get_char_position(const char *text, int idx, int char_size, const RFont *font) {
    sf::Text sf_text = {};
    sf_text.setFont(*font);
    sf_text.setString(text);
    sf_text.setCharacterSize(char_size);

    sf::Vector2f pos = sf_text.findCharacterPos(idx);
    return {pos.x, pos.y};
}

void Renderer::apr_draw_circle(Vec2d pos, double rad, int granularity) {
    if (!state->appearence) return;
    pos += state->offset;
    cur_verticies.clear();

    double angle_granularity = M_PI * 2 / (double) granularity;

    for (int i = 0; i < granularity; ++i) {
        double ang = angle_granularity * i;

        Vec2d p_offset(sin(ang) * rad, cos(ang) * rad);
        Vec2d scr_pos = pos + p_offset;
    
        cur_verticies.push_back(state->appearence->vertex(scr_pos, pos));
    }

    RMode *rmode = state->appearence->get_render_mode();
    if (rmode) {
        state->target->draw(&cur_verticies[0], cur_verticies.size(), sf::TriangleFan, *rmode);
    } else {
        state->target->draw(&cur_verticies[0], cur_verticies.size(), sf::TriangleFan, state->rmode);
    }
}

void Renderer::apr_draw_rectangle(Vec2d pos, const Vec2d size) {
    if (!state->appearence) return;
    pos += state->offset;
    cur_verticies.clear();

    cur_verticies.push_back(state->appearence->vertex({pos.x()           , pos.y()           }, {0, 0}));
    cur_verticies.push_back(state->appearence->vertex({pos.x() + size.x(), pos.y()           }, {size.x(), 0}));
    cur_verticies.push_back(state->appearence->vertex({pos.x() + size.x(), pos.y() + size.y()}, {size.x(), size.y()}));
    cur_verticies.push_back(state->appearence->vertex({pos.x()           , pos.y() + size.y()}, {0, size.y()}));


    RMode *rmode = state->appearence->get_render_mode();
    if (rmode) {
        state->target->draw(&cur_verticies[0], cur_verticies.size(), sf::Quads, *rmode);
    } else {
        state->target->draw(&cur_verticies[0], cur_verticies.size(), sf::Quads, state->rmode);
    }
}

RGBA Renderer::get_pixel_color(Vec2d pos, const RTexture *texture) {
    RRendTexture inter;
    inter.create(1, 1);

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setPosition(-pos.x(), -pos.y());

    auto size = sprite.getLocalBounds();
    sprite.setOrigin(0, size.height);

    sprite.setScale(1, -1);
    inter.draw(sprite);

    auto img = inter.getTexture().copyToImage();
    auto color = img.getPixel(0, 0);

    return {color.r, color.g, color.b, color.a};
}