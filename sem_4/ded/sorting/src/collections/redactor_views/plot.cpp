#include "plot.h"

#include "engine/Resources.h"
#include "redactor/engine.h"


v_Plottet::v_Plottet(const ViewBody &body, RColor _background_color, Range2f _range) :
v_Highlighter(body),
range(_range),

x_scale(body.size.x()  / (_range.x_max - _range.x_min)),
y_scale(body.size.y() / (_range.y_max - _range.y_min)),

background_color(_background_color),
draw_color({15, 15, 15})
{
    if (range.x_min > range.x_max) {
        std::swap(range.x_min, range.x_max);
    }

    if (range.y_min > range.y_max) {
        std::swap(range.y_min, range.y_max);
    }

    rend = App.engine->visual->get_renderer();

    texture = new RRendTexture();
    texture->create(body.size.x(), body.size.y());

    set_appearence(new AppearenceTexture(&texture->getTexture(), {1, 1}));

    clear();
}

v_Plottet::~v_Plottet() {
    delete appearence;
    appearence = nullptr;

    delete texture;
}

void v_Plottet::render(Renderer *renderer)  {
    v_Highlighter::render(renderer);
}

void v_Plottet::set_ranges(Range2f _range) { range = _range; }

void v_Plottet::clear() {
    fill(background_color);
}

void v_Plottet::fill(RColor color) {
    rend->push_target(texture);

    rend->draw_rectangle({0, 0}, body.size, color);

    rend->pop_target();
}

Vec2d v_Plottet::to_window_coords(Vec2d point) {
    point.content[0] *= x_scale;
    point.content[1] *= y_scale;

    point.content[0] -= range.x_min * x_scale;
    point.content[1] -= range.y_min * y_scale;

    return point;
}

void v_Plottet::draw_line(Vec2d p1, Vec2d p2) {
    rend->push_target(texture);

    rend->draw_line(to_window_coords(p1), to_window_coords(p2), draw_color);

    rend->pop_target();
}

void v_Plottet::draw_vector(Vec2d p1, Vec2d p2, double head_angle, double head_length) {
    draw_line(p1, p2);

    p1 = to_window_coords(p1);
    p2 = to_window_coords(p2);

    Vec2d head_vec = (p1 - p2).normal() * head_length;

    Vec2d pt1 = p2 + head_vec.roted(-head_angle);
    Vec2d pt2 = p2;
    Vec2d pt3 = p2 + head_vec.roted(+head_angle);

    rend->push_target(texture);

    rend->draw_triangle(pt1, pt2, pt3, draw_color);

    rend->pop_target();
}

void v_Plottet::draw_coord_lines() {
    draw_vector({range.x_min, 0}, {range.x_max, 0});
    draw_vector({0, range.y_min}, {0, range.y_max});
}

void v_Plottet::draw_graph(double (*func)(double), int xs_per_pixel) {
    int ox_steps = body.size.x() * xs_per_pixel;
    double x_step = (range.x_max - range.x_min) / ox_steps;

    Vec2d p1 = {range.x_min, func(range.x_min)};
    Vec2d p2 = {range.x_min, func(range.x_min)};

    for (int i = 0; i < ox_steps; ++i) {
        double x = range.x_min + x_step * i;
        double y = func(x);

        p2 = {x, y};

        draw_line(p1, p2);

        p1 = p2;
    }
}

void v_Plottet::graphicate(double (*func)(double), int xs_per_pixel, bool to_fill) {
    clear();
    
    draw_coord_lines();
    draw_graph(func, xs_per_pixel);
}
