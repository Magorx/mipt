#ifndef SFML_EXTENSION
#define SFML_EXTENSION

#include <SFML/Graphics.hpp>
#include <cmath>

#include "graphics_lib_interface.h"

const float SFML_EXT_EPS = 1e-6;

struct Range2f {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
};

class CoordinateSystem {
    GextVector position;
    float width;
    float height;

    Range2f range;

    float angle;

    float x_scale;
    float y_scale;

    sf::RenderWindow  *sf_window;
    sf::RenderTexture  sf_texture;
    sf::Sprite         sf_sprite;
    
    GextColor draw_color;
    GextColor fill_color;

public:
    CoordinateSystem(GextVector _position  = {0, 0}, float _width = 150, float _height = 100, 
                     sf::RenderWindow *_sf_window = nullptr,
                     Range2f _range = {-1, 1, -1, 1},
                     float _angle = 0);

    void clear();

    void set_position  (GextVector _position);
    void set_ranges(Range2f _range);
    void set_angle (float _angle);

    void rotate(float _angle);

    void set_window(sf::RenderWindow *window);
    
    void set_draw_color(GextColor color);
    void set_fill_color(GextColor color);

    void fill();
    void draw_line(GextVector p1, GextVector p2);
    void draw_vector(GextVector p1, GextVector p2, float head_angle = M_PI / 8, float head_length_coef = 0.05, float fixed_len=NAN);
    
    void draw_coord_lines();
    void draw_graph(float (*func)(float), int xs_per_pixel = 10);
    void draw_graph_parametric(float (*func_x)(float), float (*func_y)(float), GextColor (*func_color)(float), float t_min, float t_max, int steps);

    void graphicate(float (*func)(float), int xs_per_pixel = 10, bool to_fill = true);
    void graphicate(float (*func)(float), int xs_per_pixel, GextColor _fill_color, GextColor _draw_color, bool to_fill);

    void graphicate_parametric(float (*func_x)(float), float (*func_y)(float), GextColor (*func_color)(float), float t_min, float t_max, int steps);

    void flush();

    GextVector to_window_coords(GextVector point);
};

GextVector vector_rotate(GextVector vec, float angle);

#endif // SFML_EXTENSION