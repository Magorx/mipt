#pragma once


#include "utils.h"
#include "redactor/canvas/canvas.h"
#include "visual/renderer/renderer.h"


struct Range2d {
    double x_min;
    double x_max;
    double y_min;
    double y_max;

    Range2d(double x_min, double x_max, double y_min, double y_max):
    x_min(x_min),
    x_max(x_max),
    y_min(y_min),
    y_max(y_max)
    {}

    Range2d(Vec2d min, Vec2d max):
    x_min(min.x()),
    x_max(max.x()),
    y_min(min.y()),
    y_max(max.y())
    {}
};


class v_Plottet : public v_Highlighter {
    Range2d range;

    double angle;

    double x_scale;
    double y_scale;

    RRendTexture *texture;
    Renderer *rend;
    
    RColor background_color;
    RColor draw_color;

public:
    v_Plottet(const ViewBody &body, RColor _background_color, Range2d _range = {-1, 1, -1, 1});
    
    ~v_Plottet();
    
    virtual void render(Renderer *renderer) override;

    void clear();

    void set_ranges(Range2d _range);
    void set_draw_color(RColor color);
    RColor get_draw_color();

    void fill(RColor color);
    void draw_point(Vec2d p, double radius = 3);
    void draw_line(Vec2d p1, Vec2d p2);
    void draw_vector(Vec2d p1, Vec2d p2, double head_angle = M_PI / 8, double head_length = 15);
    
    void draw_coord_lines(RColor color={0, 0, 0});

    void draw_graph(double (*func)(double), int xs_per_pixel = 10);
    void draw_graph(const std::vector<Vec2d> &points, bool to_draw_points = true, bool to_draw_columns = true);
    void draw_graph(Vec2d (*func)(double), Vec2d t_range, double step, RColor (*func_color)(double)=nullptr);

    void graphicate(double (*func)(double), int xs_per_pixel = 10);
    void graphicate(const std::vector<Vec2d> &points, bool to_draw_points = true, bool to_draw_columns = true);
    void graphicate(Vec2d (*func)(double), Vec2d t_range, double step, RColor (*func_color)(double)=nullptr);

    void flush_to_window();

    Vec2d to_window_coords(Vec2d point);
};


class PlotterColorSet {
    v_Plottet &plotter;
    RColor prev_color;

public:
    PlotterColorSet(v_Plottet &plotter, RColor color);
    ~PlotterColorSet();
};
