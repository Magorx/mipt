#ifndef GLIB_INTERFACE
#define GLIB_INTERFACE

#include <SFML/Graphics.hpp>

typedef sf::Vector2f GextVector;
typedef sf::Vector2f GlibVector;
#define GEXT_GLIB_VECTORS_SAME_TYPE

typedef sf::Color GextColor;
typedef sf::Color GlibColor;
#define GEXT_GLIB_COLORS_SAME_TYPE

#ifndef GEXT_GLIB_VECTORS_SAME_TYPE
inline GlibVector gext_to_glib(GextVector vec) {
    return GlibVector(vec.x, vec.y);
}
#else
inline GlibVector gext_to_glib(GlibVector vec) {
    return vec;
}
#endif // GEXT_GLIB_VECTORS_SAME_TYPE

#ifndef GEXT_GLIB_COLORS_SAME_TYPE
inline GlibColor gext_to_glib(GextColor color) {
    return GlibColor(color.r, color.g, color.b);
}
#else
inline GlibColor gext_to_glib(GlibColor color) {
    return color;
}
#endif

template <typename T, typename G>
T gext_to_glib(T gext, G glib) {
    
}


void inline sync_canvas(sf::Sprite &sprite, GextVector position, float angle) {
    sprite.setPosition(gext_to_glib(position));
    sprite.setRotation(angle);
}

void inline initialize_canvas(sf::RenderTexture &render_texture, sf::Sprite &sprite,
                              float width, float height, float angle, GextVector position) {
    render_texture.create(width, height);
    sprite.setTexture(render_texture.getTexture());

    sprite.setOrigin(width / 2, height / 2);
    sprite.setPosition(gext_to_glib(position) + GlibVector(width / 2, height / 2));
    sprite.setRotation(angle);

    sync_canvas(sprite, position, angle);
}

void inline texture_draw_line(sf::RenderTexture &texture, const GextVector p1, const GextVector p2, const GextColor color) {
    sf::Vertex vertices[] =  {{gext_to_glib(p1), gext_to_glib(color)}, {gext_to_glib(p2), gext_to_glib(color)}};
    size_t vertices_count = sizeof(vertices) / sizeof(sf::Vertex);

    texture.draw(vertices, vertices_count, sf::PrimitiveType::Lines);
}

void inline texture_draw_rectangle(sf::RenderTexture &texture, const GextVector position, const GextVector size, const GextColor color) {
    sf::RectangleShape rectangle(gext_to_glib(size));
    rectangle.setFillColor(gext_to_glib(color));
    //rectangle.setPosition(gext_to_glib(position));

    texture.draw(rectangle);
}

void inline texture_draw_triangle(sf::RenderTexture &texture, const GextVector p1, const GextVector p2, const GextVector p3, const GextColor color) {
    sf::Vertex vertices[] =  {{gext_to_glib(p1), color}, 
                              {gext_to_glib(p2), color}, 
                              {gext_to_glib(p3), color}};
    size_t vertices_count = sizeof(vertices) / sizeof(sf::Vertex);

    texture.draw(vertices, vertices_count, sf::PrimitiveType::Triangles);
}

void inline flush_to_window(sf::RenderWindow *window, sf::Sprite sprite) {
    if (window) window->draw(sprite);
}

#endif // GLIB_INTERFACE