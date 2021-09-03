#ifndef SFML_EXTENSION
#define SFML_EXTENSION

#include <SFML/Graphics.hpp>
#include <cmath>
// #include "matrix4.h"

const float SFML_EXT_EPS = 1e-6;

class CoordinateSystem {
    sf::Vector2f bias;
    float width;
    float height;

    float x_coef;
    float y_coef;

    float x_min;
    float x_max;
    float y_min;
    float y_max;

    float angle;

    sf::RenderWindow  *sf_window;
    sf::RenderTexture  sf_texture;
    sf::Sprite         sf_sprite;
    
    sf::Color draw_color;
    sf::Color fill_color;

public:
    CoordinateSystem(sf::Vector2f _bias  = {0, 0}, float _width = 150, float _height = 100);

    void clear();

    void set_bias  (sf::Vector2f _bias);
    void set_ranges(float _x_min, float _x_max, float _y_min, float _y_max);
    void set_angle (float _angle);

    void rotate(float _angle);

    void set_window(sf::RenderWindow *window);
    
    void set_draw_color(sf::Color color);
    void set_fill_color(sf::Color color);

    void fill();
    void draw_line(sf::Vector2f p1, sf::Vector2f p2);
    void draw_vector(sf::Vector2f p1, sf::Vector2f p2, float head_angle = M_PI / 8, float head_length_coef = 0.05);
    
    void draw_coord_lines();
    void draw_graph(float (*func)(float), int xs_per_pixel = 10);
    void draw_graph_parametric(float (*func_x)(float), float (*func_y)(float), sf::Color (*func_color)(float), float t_min, float t_max, int steps);

    void graphicate(float (*func)(float), int xs_per_pixel = 10, bool to_fill = true);
    void graphicate(float (*func)(float), int xs_per_pixel, sf::Color _fill_color, sf::Color _draw_color, bool to_fill);

    void graphicate_parametric(float (*func_x)(float), float (*func_y)(float), sf::Color (*func_color)(float), float t_min, float t_max, int steps);

    void flush_to_window();

    sf::Vector2f to_window_coords(sf::Vector2f point);
};

template <typename T>
sf::Vector2<T> sf_vector_rotate(sf::Vector2<T> vec, float angle);

#endif // SFML_EXTENSION