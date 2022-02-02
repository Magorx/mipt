#pragma once


#include "mouse_catcher.h"
#include "button.h"
#include "horizontal_layout.h"
#include "stretcher.h"
#include "drag_tab.h"
#include "toggler.h"
#include "repeater.h"

#include "visual/style/utility_bar_style.h"


const int PX_UTIL_HEADER_SIZE = 32;
const int PX_UTIL_BUTTON_SIZE = 20;
const int PX_UTIL_PADDING = (PX_UTIL_HEADER_SIZE - PX_UTIL_BUTTON_SIZE) / 2;
const int PX_UTIL_MIDDLE_STUB_SIZE = 16;


class v_UtilityTab : public v_Highlighter {
    v_HorizontalLayout *buttons_layout;

    v_Button *b_close;
    v_Button *b_hide;
    v_DragPad *v_drag_pod;
    v_Stretcher *v_options;

    v_Button *v_accessory;

    v_Highlighter *v_l_corner;
    v_Highlighter *v_r_corner;
    v_Repeater    *v_middle;
    v_Repeater    *v_under;

public:

    static const Color close_button_color;
    static const Color hide_button_color;
    static const Color drag_tab_color;

    // v_UtilityTab(Vec2d size, SmartColor *main_color);
    v_UtilityTab(Vec2d size, UtilityBarStyle *style);

    virtual bool is_inside(const Vec2d &click) override;

    inline v_DragPad *get_drag_pod()    { return v_drag_pod; }
    inline v_Button *get_button_close() { return b_close; }
    inline v_Button *get_button_hide()  { return b_hide; }
    inline AbstractView *get_accessory() { return v_accessory; }
    v_Stretcher *get_options() { return v_options; }

};
