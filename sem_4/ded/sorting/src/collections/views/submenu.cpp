#include "submenu.h"
#include "../Collections.h"


v_Submenu::v_Submenu(Type type, const ViewBody &body_, SmartColor *color, const char *label, std::vector<const char*> button_labels, SmartColor *button_color, RGBA text_color, SmartColor *back_color, int char_size, Vec2d button_scale):
v_Highlighter(body_, color),
align({{0, 1}, {1, 2}})
{
    add_label(label, char_size, text_color);

    menu = new v_Menu(button_labels, button_color, text_color, back_color, char_size, button_scale);
    menu->deactivate();
    menu->e_toggle_activity.add(new HideableActivityToggleAcceptor(menu));
    
    if (type == Type::on_hover) {
        e_mouse_move.add(new a_OnHoverToggler(this, menu), false);
    } else {
        e_mouse_press.add(new a_OnPressToggler(this, menu), false);
    }
    
    add_subview(menu);

    refit();
}

void v_Submenu::refit() {
    v_Highlighter::refit();
    
    // menu->set_min_size({body.size.x(), 0});
    menu->fit(align.position, align.size);
    menu->refit();
}

AbstractView *v_Submenu::operator[](size_t i) {
    return (*menu)[i];
}
