#include "sfml_extension.h"

CoordinateSystem::CoordinateSystem(GextVector _position, float _width, float _height,
                                   sf::RenderWindow *_sf_window,
                                   Range2f _range,
                                   float _angle) :
position(_position),

width (_width),
height(_height),

range(_range),

x_scale(_width  / (_range.x_max - _range.x_min)),
y_scale(_height / (_range.y_max - _range.y_min)),

angle(_angle),

sf_window(_sf_window),
sf_texture(),
sf_sprite(),

draw_color({  0,   0,   0}),
fill_color({225, 225, 225})
{
    if (range.x_min > range.x_max + SFML_EXT_EPS) {
        std::swap(range.x_min, range.x_max);
    }
    if (range.y_min > range.y_max + SFML_EXT_EPS) {
        std::swap(range.y_min, range.y_max);
    }

    initialize_canvas(sf_texture, sf_sprite, width, height, angle, position);
}

void CoordinateSystem::set_position(GextVector _position) {
    position = _position;
    sync_canvas(sf_sprite, position, angle);
}

void CoordinateSystem::set_ranges(Range2f _range) {
    range = _range;

    x_scale = width  / (range.x_max - range.x_min);
    y_scale = height / (range.y_max - range.y_min);
}

void CoordinateSystem::set_angle(float _angle) {
    angle = _angle;
    sync_canvas(sf_sprite, position, angle);
}

void CoordinateSystem::rotate(float _angle) {
    angle += _angle;
    sync_canvas(sf_sprite, position, angle);
}

void CoordinateSystem::set_window(sf::RenderWindow *window) {
    sf_window = window;
}

void CoordinateSystem::set_draw_color(GlibColor color) {
    draw_color = color;
}

void CoordinateSystem::set_fill_color(GlibColor color) {
    fill_color = color;
}

void CoordinateSystem::flush() {
    flush_to_window(sf_window, sf_sprite);
}

void CoordinateSystem::clear() {
    sf_texture.clear();
}

void CoordinateSystem::fill() {
    texture_draw_rectangle(sf_texture, {0, 0}, {width, height}, fill_color);
}

void CoordinateSystem::draw_line(GextVector p1, GextVector p2) {
    p1 = to_window_coords(p1);
    p2 = to_window_coords(p2);

    texture_draw_line(sf_texture, p1, p2, draw_color);
}

void CoordinateSystem::draw_vector(GextVector p1, GextVector p2, float head_angle, float head_length_coef, float fixed_len) {
    draw_line(p1, p2);

    p1 = to_window_coords(p1);
    p2 = to_window_coords(p2);

    GextVector head_vec(0, 0);
    if (fixed_len == fixed_len) {
        head_vec = (p1 - p2);
        float l = sqrt(head_vec.x * head_vec.x + head_vec.y * head_vec.y);
        head_vec = head_vec / l * fixed_len;
    } else {
        head_vec = (p1 - p2) * head_length_coef;
    }
    
    texture_draw_triangle(sf_texture, 
                          p2 + vector_rotate(head_vec, -head_angle),
                          p2,
                          p2 + vector_rotate(head_vec, +head_angle),
                          draw_color);
}

GextVector CoordinateSystem::to_window_coords(GextVector point) {
    point.x *= x_scale;
    point.y *= y_scale;

    point.x -= range.x_min * x_scale;
    point.y -= range.y_min * y_scale;

    return point;
}

void CoordinateSystem::draw_coord_lines() {
    draw_vector({range.x_min, 0}, {range.x_max, 0});
    draw_vector({0, range.y_min}, {0, range.y_max});
}

void CoordinateSystem::draw_graph(float (*func)(float), int xs_per_pixel) {
    int ox_steps = width * xs_per_pixel;
    float x_step = (range.x_max - range.x_min) / ox_steps;

    GextVector prev_coord = to_window_coords({range.x_min, func(range.x_min)});

    for (int i = 0; i < ox_steps; ++i) {
        float x = range.x_min + x_step * i;
        float y = func(x);
        GextVector coord = to_window_coords({x, y});

        texture_draw_line(sf_texture, prev_coord, coord, draw_color);

        prev_coord = coord;
    }
}

void CoordinateSystem::graphicate(float (*func)(float), int xs_per_pixel, bool to_fill) {
    clear();
    if (to_fill) fill();
    
    draw_coord_lines();
    draw_graph(func, xs_per_pixel);

    flush();
}

void CoordinateSystem::draw_graph_parametric(float (*func_x)(float), float (*func_y)(float), GlibColor (*func_color)(float), float t_min, float t_max, int steps) {
    float t_step = (t_max - t_min) / steps;

    GextVector prev_coord = to_window_coords({func_x(t_min), func_y(t_min)});

    for (int i = 0; i < steps; ++i) {
        float t = t_min + t_step * i;
        float x = func_x(t);
        float y = func_y(t);
        GextVector coord = to_window_coords({x, y});

        texture_draw_line(sf_texture, prev_coord, coord, func_color(t));

        prev_coord = coord;
    }
}

void CoordinateSystem::graphicate_parametric(float (*func_x)(float), float (*func_y)(float), GlibColor (*func_color)(float), float t_min, float t_max, int steps) {
    clear();
    fill();
    
    draw_coord_lines();
    draw_graph_parametric(func_x, func_y, func_color, t_min, t_max, steps);

    flush();
}

GextVector vector_rotate(GextVector vec, float angle) {
    float ang_sin = sin(angle);
    float ang_cos = cos(angle);

    return {ang_cos * vec.x - ang_sin * vec.y, ang_sin * vec.x + ang_cos * vec.y};
} 
