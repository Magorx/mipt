#include "dialog_window.h"
#include "engine/moga_engine.h"

#include "collections/views/magnetic.h"
#include "collections/redactor_views/color_picker.h"


v_DialogWindow::v_DialogWindow(const char *name, double width, double padding, double center_offset, WindowStyle *style) :
v_Window(name, {0, {width, 0}}, style, true),
v_stretcher(v_Stretcher::Y({0, {width, 0}})),
center_offset(center_offset),
padding(padding),
first_selectable(nullptr)
{
    App.engine->centrize_view_body(body);
    toggle_hide_button();

    get_content()->add_subview(v_stretcher);

    v_stretcher->set_zerify_normal_coord(false);
    v_stretcher->add_placehodler(padding);
}

void v_DialogWindow::fit_content_to_fields() {
    get_content()->get_body().size.content[1] = v_stretcher->get_body().size[1];
    fit_frame_to_content();
}

v_TextField *v_DialogWindow::add_info_line(const char *text) {
    v_TextField *field  = new v_TextField({0, 0}, StdStyle::Text::basic()->negative(), 0, 0, true, false);
    field->set_string(text, true);
    Vec2d rect_pos = {get_content()->get_body().size.x() / 2 - field->get_body().size.x() / 2, 0};

    v_Highlighter *rect = new v_Highlighter({rect_pos, {field->get_body().size.x(), field->get_body().size.y()}});
    rect->add_subview(field);

    v_stretcher->add_subview(rect);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    return field;
}

v_TextField *v_DialogWindow::add_field(const char *field_name, double max_ans_length, const char *after_name) {

    v_TextField *field  = new v_TextField({0, 0}, StdStyle::Text::basic()->negative(), 0, 0, true, false);
    field->set_string(field_name, true);
    Vec2d rect_pos = {get_content()->get_body().size.x() / 2 - field->get_body().size.x() + center_offset, 0};

    v_TextField *answer = new v_TextField({{field->get_body().size.x(), 0}, max_ans_length});
    
    v_Highlighter *rect = new v_Highlighter({rect_pos, {field->get_body().size.x() + answer->get_body().size.x(), field->get_body().size.y()}});
    rect->add_subview(field);
    rect->add_subview(answer);
    
    if (after_name) {
        v_TextField *aftertext  = new v_TextField({0, 0}, StdStyle::Text::basic()->negative(), 0, 0, true, false);
        aftertext->set_string(after_name, true);
        aftertext->get_body().position.content[0] = answer->get_body().position[0] + answer->get_body().size[0];
        rect->add_subview(aftertext);
    }

    // rect->set_appearence(new AppearenceColor({100, 100, 200, 200}));
    v_stretcher->add_subview(rect);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    if (!first_selectable && answer->is_selectable()) {
        first_selectable = answer;
    }

    return answer;
}

v_Button *v_DialogWindow::add_text_button(const char *button_name,
                                          bool to_centrize,
                                          MouseReactionStyle *button_style,
                                          TextStyle *text_style) {
    v_Button *button = new v_Button(button_name, button_style, text_style, PX_BUTTON_SIZE_PADDING);

    if (to_centrize) {
        button->get_body().position.content[0] = get_content()->get_body().size.x() / 2 - button->get_body().size.x() / 2;
    } else {
        button->get_body().position.content[0] = (get_content()->get_body().size.x() / 2 - button->get_body().size.x() / 2);
    }

    v_stretcher->add_subview(button);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    if (!first_selectable && button->is_selectable()) {
        first_selectable = button;
    }

    return button;
}

v_Button *v_DialogWindow::add_accept_button (const char *button_name, bool to_centrize, TextStyle *text_style) {
    return add_text_button(button_name,
                           to_centrize,
                           App.add_style(StdStyle::Button::green()),
                           text_style);
}

v_Button *v_DialogWindow::add_decline_button(const char *button_name, bool to_centrize, TextStyle *text_style) {
    return add_text_button(button_name,
                           to_centrize,
                           App.add_style(StdStyle::Button::red()),
                           text_style);
}

void v_DialogWindow::make_closing_field(v_TextField *field, v_Button *acception_button) {
    field->e_text_changed.add(new DialogWindowCloseByInputDoneReaction(this, acception_button));
}

void v_DialogWindow::make_closing_button(v_Button *closer) {
    closer->e_clicked.add(new AVCloseGenerator<Event::Clicked>(closer));
    closer->e_close.add(new AVCloseAcceptor(this));
}

void v_DialogWindow::select_first_field() {
    if (first_selectable) {
        first_selectable->select(true);
    }
}

v_Magnetic *v_DialogWindow::add_slider(const char *name, double length, RColor color) {
    v_TextField *field  = new v_TextField({0, 0}, StdStyle::Text::basic()->negative(), 0, 0, true, false);
    field->set_string(name, true);
    Vec2d rect_pos = {get_content()->get_body().size.x() / 2 - field->get_body().size.x() + center_offset, 0};

    const double STICK_BOUND = 4;

    v_Magnetic *answer = new v_Magnetic(
        {{field->get_body().size.x(), field->get_body().size.y() / 2}, {length, PX_MAG_DOT}},
        {0, Vec2d{PX_MAG_DOT, PX_MAG_DOT} + Vec2d{0, STICK_BOUND}}
    );
    answer->get_dot()->set_appearence(App.add_appr(new AppearenceTexture(App.texture.stick_vert, {1, 1}, {-answer->get_dot()->get_body().size.x() / 2, -STICK_BOUND / 2})));
    answer->toggle_x_restriction();
    answer->set_appearence(App.add_appr(new AppearenceColor(color)));
    
    v_Highlighter *rect = new v_Highlighter({rect_pos, {field->get_body().size.x() + answer->get_body().size.x(), field->get_body().size.y()}});
    rect->add_subview(field);
    rect->add_subview(answer);

    v_stretcher->add_subview(rect);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    if (!first_selectable && answer->is_selectable()) {
        first_selectable = answer;
    }

    return answer;
}

v_ColorPicker *v_DialogWindow::add_color_picker(const double height) {
    v_ColorPicker *answer = new v_ColorPicker({0, {v_stretcher->get_body().size.x(), height}}, nullptr);

    Vec2d rect_pos = {0, 0};

    v_Highlighter *rect = new v_Highlighter({rect_pos, {answer->get_body().size.x() + answer->get_body().size.x(), answer->get_body().size.y()}});
    rect->add_subview(answer);

    v_stretcher->add_subview(rect);
    v_stretcher->add_placehodler(padding);

    fit_content_to_fields();

    if (!first_selectable && answer->is_selectable()) {
        first_selectable = answer;
    }

    return answer;
}

v_DialogWindow *v_DialogWindow::Error(double width, const char *message, const char *name, const char *button_text) {
    auto dw = new v_DialogWindow(name, width);
    dw->add_info_line(message);
    auto close_button = dw->add_decline_button(button_text, true);
    dw->make_closing_button(close_button);

    dw->select_first_field();

    return dw;
}

DialogWindowCloseByInputDoneReaction::DialogWindowCloseByInputDoneReaction(v_DialogWindow *dialog_window, v_Button *acception_button):
EventAcceptor(dialog_window),
acception_button(acception_button)
{}

EventAccResult DialogWindowCloseByInputDoneReaction::operator()(const Event::TextChanged &event, const EventAccResult *) {
    if (event.input_complete) {
        if (acception_button) {
            acception_button->e_clicked.emit({});
        }

        acceptor->set_to_delete(true);
        return EventAccResult::cont;
    }
    
    return EventAccResult::none;
}
