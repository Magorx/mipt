#pragma once

const int SCR_W  = 1200;
const int SCR_H  = 800;

#include "redactor/engine.h"
#include "scene_generation.h"
#include "responses.h"


double parab(double x) {
    return x * x;
}

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

    auto plotter = new v_Plottet({50, 200}, {240, 240, 240}, {-10, 10, -10, 10});
    moga.add_view(plotter);   

    // plotter->draw_vector({0, 0}, {0.5, 0.5});
    plotter->graphicate(parab);

}
