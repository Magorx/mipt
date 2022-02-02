#include "menu.h"


v_Menu::v_Menu(const std::vector<const char*> &button_labels, SmartColor *button_color, RGBA text_color, SmartColor *back_color, int char_size, Vec2d button_scale):
v_VerticalLayout({{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, 0, nullptr, back_color),
// button_color(button_color),
// text_color(text_color),
char_size(char_size),
button_scale(button_scale),
min_size(body.size)
{
    labels.resize(button_labels.size());
    for (size_t i = 0; i < labels.size(); ++i) {
        labels[i] = strdup(button_labels[i]);

        v_Button *ab = new v_Button({{0, 0}, {0, 0}}, button_color);
        ab->add_label(labels[i], char_size, text_color);
        layout_add(ab, button_scale.y());

        // ab->e_mouse_press.add(new AVMissPressBlocker(ab));
        // ab->e_mouse_release.add(new AVMissReleaseBlocker(ab));
        // ab->e_mouse_move.add(new AVMissMoveBlocker(ab));
    }

    refit();
}

v_Menu::~v_Menu() {
    for (auto label : labels) {
        free(label);
    }
}

AbstractView *v_Menu::operator[](size_t i) {
    assert(i < labels.size() && "trying to get button[i] from submenu of smaller size");
    return subviews[i];
}

void v_Menu::refit() {
    AbstractView::refit();
    recalculate_size();
    recalculate_fit_body();
    layout_refit();
}

void v_Menu::recalculate_size() {
    double sum_y_size = 0;
    double max_x_size = 0;

    for (size_t i = 0; i < labels.size(); ++i) {
        const char *label = labels[i];

        Vec2d text_size = Renderer::get_text_size(label, char_size);
        
        max_x_size = fmax(max_x_size, text_size.x());
        sum_y_size += text_size.y();
    }

    body.size = Vec2d(max_x_size, sum_y_size) * button_scale;
    body.size.content[0] = fmax(body.size.content[0], min_size.content[0]);
    body.size.content[1] = fmax(body.size.content[1], min_size.content[1]);
}