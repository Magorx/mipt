#pragma once


#include "utils.h"
#include "redactor/canvas/canvas.h"
#include "visual/renderer/renderer.h"


struct Range2f {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};


class v_Plottet : public v_Highlighter {
    Range2f range;

    double angle;

    double x_scale;
    double y_scale;

    RRendTexture *texture;
    Renderer *rend;
    
    RColor background_color;
    RColor draw_color;

public:
    v_Plottet(const ViewBody &body, RColor _background_color, Range2f _range = {-1, 1, -1, 1});
    
    ~v_Plottet();
    
    virtual void render(Renderer *renderer) override;

    void clear();

    void set_ranges(Range2f _range);

    void fill(RColor color);
    void draw_line(Vec2d p1, Vec2d p2);
    void draw_vector(Vec2d p1, Vec2d p2, double head_angle = M_PI / 8, double head_length = 15);
    
    void draw_coord_lines();
    void draw_graph(double (*func)(double), int xs_per_pixel = 10);
    void draw_graph_parametric(double (*func_x)(double), double (*func_y)(double), RColor (*func_color)(double), double t_min, double t_max, int steps);

    void graphicate(double (*func)(double), int xs_per_pixel = 10, bool to_fill = true);
    void graphicate(double (*func)(double), int xs_per_pixel, RColor _fill_color, RColor _draw_color, bool to_fill);

    void graphicate_parametric(double (*func_x)(double), double (*func_y)(double), RColor (*func_color)(double), double t_min, double t_max, int steps);

    void flush_to_window();

    Vec2d to_window_coords(Vec2d point);
};
