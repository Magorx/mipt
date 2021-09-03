#include "sfml_extension.h"

CoordinateSystem::CoordinateSystem(sf::Vector2f _bias, float _width, float _height) :
bias(_bias),
width(_width),
height(_height),

x_min(-1),
x_max( 1),
y_min(-1),
y_max( 1),

x_coef(1),
y_coef(1),

angle(0),

sf_window(nullptr),
sf_texture(),
sf_sprite(),

draw_color({0, 0, 0}),
fill_color({225, 225, 225})
{
    if (x_min > x_max + SFML_EXT_EPS) {
        std::swap(x_min, x_max);
    }
    if (y_min > y_max + SFML_EXT_EPS) {
        std::swap(y_min, y_max);
    }

    x_coef = width  / (x_max - x_min);
    y_coef = height / (y_max - y_min);

    sf_texture.create(width, height);
    sf_sprite.setTexture(sf_texture.getTexture());

    sf_sprite.setOrigin(width / 2, height / 2);
    sf_sprite.setPosition(bias + sf::Vector2f(width / 2, height / 2));
    sf_sprite.setRotation(angle);
}

void CoordinateSystem::set_bias(sf::Vector2f _bias) {
    bias = _bias;
    sf_sprite.setPosition(bias + sf::Vector2f(width / 2, height / 2));
}

void CoordinateSystem::set_ranges(float _x_min, float _x_max, float _y_min, float _y_max) {
    x_min = _x_min;
    x_max = _x_max;
    y_min = _y_min;
    y_max = _y_max;

    x_coef = width  / (x_max - x_min);
    y_coef = height / (y_max - y_min);
}

void CoordinateSystem::set_angle(float _angle) {
    angle = _angle;
    sf_sprite.setRotation(angle);
}

void CoordinateSystem::rotate(float _angle) {
    angle += _angle;
    sf_sprite.setRotation(angle);
}

void CoordinateSystem::set_window(sf::RenderWindow *window) {
    sf_window = window;
}

void CoordinateSystem::set_draw_color(sf::Color color) {
    draw_color = color;
}

void CoordinateSystem::set_fill_color(sf::Color color) {
    fill_color = color;
}

void CoordinateSystem::flush_to_window() {
    if (!sf_window) {
        return;
    }

    sf_window->draw(sf_sprite);
}

void CoordinateSystem::clear() {
    sf_texture.clear();
}

void CoordinateSystem::fill() {
    sf::RectangleShape rectangle({width, height});
    rectangle.setFillColor(fill_color);

    sf_texture.draw(rectangle);
}

void CoordinateSystem::draw_line(sf::Vector2f p1, sf::Vector2f p2) {
    p1 = to_window_coords(p1);
    p2 = to_window_coords(p2);

    // printf("p1: %f %f\n", p1.x, p1.y);
    // printf("p2: %f %f\n", p2.x, p2.y);

    sf::Vertex vertices[] =  {{p1, draw_color}, {p2, draw_color}};
    size_t vertices_count = sizeof(vertices) / sizeof(sf::Vertex);

    sf_texture.draw(vertices, vertices_count, sf::PrimitiveType::Lines);
}

void CoordinateSystem::draw_vector(sf::Vector2f p1, sf::Vector2f p2, float head_angle, float head_length_coef) {
    draw_line(p1, p2);

    p1 = to_window_coords(p1);
    p2 = to_window_coords(p2);

    sf::Vector2f head_vec = (p1 - p2) * head_length_coef;

    sf::Vertex vertices[] =  {{p2 + sf_vector_rotate(head_vec, -head_angle), draw_color}, {p2, draw_color}, {p2 + sf_vector_rotate(head_vec, head_angle), draw_color}};
    size_t vertices_count = sizeof(vertices) / sizeof(sf::Vertex);

    sf_texture.draw(vertices, vertices_count, sf::PrimitiveType::Triangles);
}

sf::Vector2f CoordinateSystem::to_window_coords(sf::Vector2f point) {
    point.x *= x_coef;
    point.y *= y_coef;

    point.x -= x_min * x_coef;
    point.y -= y_min * y_coef;
    
    // point += bias;

    return point;
}

void CoordinateSystem::draw_coord_lines() {
    draw_vector({x_min, 0}, {x_max, 0});
    draw_vector({0, y_min}, {0, y_max});
}

void CoordinateSystem::draw_graph(float (*func)(float), int xs_per_pixel) {
    int ox_steps = width * xs_per_pixel;
    float x_step = (x_max - x_min) / ox_steps;

    sf::Vertex points[] = {{to_window_coords({x_min, func(x_min)}), draw_color}, 
                           {to_window_coords({x_min, func(x_min)}), draw_color}};

    for (int i = 0; i < ox_steps; ++i) {
        float x = x_min + x_step * i;
        float y = func(x);

        points[1].position = to_window_coords({x, y});

        sf_texture.draw(points, 2, sf::PrimitiveType::Lines);

        points[0].position = points[1].position;
    }
}

void CoordinateSystem::graphicate(float (*func)(float), int xs_per_pixel, bool to_fill) {
    clear();
    if (to_fill) fill();
    
    draw_coord_lines();
    draw_graph(func, xs_per_pixel);

    flush_to_window();
}

void CoordinateSystem::draw_graph_parametric(float (*func_x)(float), float (*func_y)(float), sf::Color (*func_color)(float), float t_min, float t_max, int steps) {
    float t_step = (t_max - t_min) / steps;

    sf::Vertex points[2] = {{to_window_coords({func_x(t_min), func_y(t_min)}), func_color(t_min)},
                            {to_window_coords({func_x(t_min), func_y(t_min)}), func_color(t_min)}};

    for (int i = 0; i < steps; ++i) {
        float t = t_min + t_step * i;
        
        float x = func_x(t);
        float y = func_y(t);

        points[1] = {to_window_coords({x, y}), func_color(t)};

        sf_texture.draw(points, 2, sf::PrimitiveType::Lines);

        points[0].position = points[1].position;
    }
}

void CoordinateSystem::graphicate_parametric(float (*func_x)(float), float (*func_y)(float), sf::Color (*func_color)(float), float t_min, float t_max, int steps) {
    clear();
    fill();
    
    draw_coord_lines();
    draw_graph_parametric(func_x, func_y, func_color, t_min, t_max, steps);

    flush_to_window();
}

template <typename T>
sf::Vector2<T> sf_vector_rotate(sf::Vector2<T> vec, float angle) {
    float ang_sin = sin(angle);
    float ang_cos = cos(angle);

    return {ang_cos * vec.x - ang_sin * vec.y, ang_sin * vec.x + ang_cos * vec.y};
} 
