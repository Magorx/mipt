#pragma once

const int SCR_W  = 1200;
const int SCR_H  = 800;

#include "redactor/engine.h"
#include "scene_generation.h"
#include "responses.h"


double parab(double x) {
    return x * x;
}

Vec2d param(double t) {
    double tmp = sin(16.6 * t);
    return {
        3 * cos(cos(7.32 * round(t))) * 1.2 * (1 + cos(16.6 * t)),
        3 * tmp * tmp * sin(7.32 * t)
    };
}

RColor param_color(double t) {
    int tmp = t * 10;
    int c = tmp;
    return {c, c * 2, c * 3};
}

// Vec2d param(double t) {
//     double rad = 10;
//     return {
//         rad * cos(t),
//         rad * sin(t)
//     };
// }

void initialize_photoshop(RedactorEngine &moga) {
    const double scr_width = moga.get_screen_width();
    // const double scr_height = moga.get_screen_height();

    auto appr_frame_box = new AppearenceTexture(App.texture.frame_gray);
    v_HorizontalLayout *opt_panel = new v_HorizontalLayout({{0, 0}, {scr_width, App.font.size.basic_menu + 4.0}}, {{0, 0}, {1, 1}}, 0, nullptr, nullptr);
    opt_panel->set_appearence(appr_frame_box);
    
    moga.add_view(opt_panel);

    // ==================================================================================

    auto new_canvas_button = new v_Button({0, 0}, StdStyle::Button::basic_menu());
    new_canvas_button->e_clicked.add(new AddNewCanvasReaction(&moga));
    new_canvas_button->add_label("Canvas", App.font.size.basic_menu, App.font.color.basic_menu);

    opt_panel->layout_add(new_canvas_button);
    
    // ==================================================================================

    auto plotter = new v_Plottet({50, {800, 600}}, {240, 240, 240}, Range2d{{-1, -10}, {7, 10}});
    moga.add_view(plotter);   

    // plotter->draw_vector({0, 0}, {0.5, 0.5});
    // plotter->graphicate(param, {-10, 10}, 0.001, param_color);
    std::vector<Vec2d> pts;
    for (double i = 0; i < 6; i += 0.25) {
        pts.push_back({i, i * sin(i)});
    }

    plotter->set_draw_color({255, 0, 0});
    plotter->graphicate(pts);

}
