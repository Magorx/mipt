#pragma once


#include "window.h"
#include "text_field.h"
#include "stretcher.h"

// #include "magnetic.h"
class v_Magnetic;
class v_ColorPicker;

#include "collections/styles/window.h"
#include "collections/styles/button.h"


const double PX_DIALOG_FIELD_HEIGHT = 32;
const Vec2d PX_BUTTON_SIZE_PADDING = {3, 0};


class DialogWindowCloseByInputDoneReaction;


class v_DialogWindow : public v_Window {
    friend DialogWindowCloseByInputDoneReaction;

    v_Stretcher *v_stretcher;
    double center_offset;
    double padding;

    AbstractView *first_selectable;

public:
    v_DialogWindow(const char *name, double width, double padding = 5, double center_offset = 0, WindowStyle *style = App.add_style(StdStyle::Window::dialog()));

    static v_DialogWindow *Error(double width, const char *message, const char *name = "Error", const char *button_text = "Dismiss");

    v_TextField *add_info_line(const char *text);

    v_TextField *add_field(const char *field_name, double max_ans_length = 100, const char *after_name = nullptr);

    v_Button *add_text_button(const char *button_name,
                              bool to_centrize = false,
                              MouseReactionStyle *button_style = App.add_style(StdStyle::Button::blue()),
                              TextStyle *text_style = App.add_style(StdStyle::Text::basic()));

    v_Button *add_accept_button (const char *button_name, bool to_centrize = false, TextStyle *style = App.add_style(StdStyle::Text::basic()->negative()));
    v_Button *add_decline_button(const char *button_name, bool to_centrize = false, TextStyle *style = App.add_style(StdStyle::Text::basic()->negative()));

    void make_closing_button(v_Button *closer);
    void make_closing_field(v_TextField *field, v_Button *acception_button = nullptr);

    v_Magnetic *add_slider(const char *name, double length = 100, RColor color = App.color.slider.basic);

    v_ColorPicker *add_color_picker(const double height);

    void select_first_field();

    void fit_content_to_fields();
};

class DialogWindowCloseByInputDoneReaction : public EventAcceptor<v_DialogWindow, Event::TextChanged> {
    v_Button *acception_button;
public:
    DialogWindowCloseByInputDoneReaction(v_DialogWindow *dialog_window, v_Button *acception_button = nullptr);

    EventAccResult operator()(const Event::TextChanged &event, const EventAccResult *cur_res = nullptr) override;
};
