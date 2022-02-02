#pragma once


#include "highlighter.h"
#include "collections/styles/text.h"
#include "utils/text_line/line.h"


const double PX_TF_PADDING = 3;
const double SC_TF_BLINK = 0.5;


class TextEnterAcceptor;
class KeyDownTextFieldAcceptor;
class KeyUpTextFieldAcceptor;
class TextFieldMousePressAcceptor;
class TextFieldMouseReleaseAcceptor;
class TextFieldMouseMoveAcceptor;


class v_TextField : public v_Highlighter {
    Line line;
    TextStyle *style;

    v_Highlighter *v_content;
    v_Highlighter *v_cursor;
    v_Highlighter *v_selection;

    bool redactable;
    bool insert = false;
    
    int selection_online = 0;

    friend TextEnterAcceptor;
    friend KeyDownTextFieldAcceptor;
    friend KeyUpTextFieldAcceptor;
    friend TextFieldMousePressAcceptor;
    friend TextFieldMouseReleaseAcceptor;
    friend TextFieldMouseMoveAcceptor;

    Vec2d char_pos(int idx);

public:
    v_TextField(const ViewBody &body, TextStyle *style = App.add_style(StdStyle::Text::basic()),
               RColor frame_color = App.color.text_field.basic.frame,
               RColor content_color = App.color.text_field.basic.content, 
               bool autofit_to_text_size = true, bool redactable = true,
               bool autofit_text_size = false);
    virtual ~v_TextField();

    virtual void render(Renderer *renderer) override;
    virtual void refit() override;

    virtual void select(bool tabbed = false) override;
    virtual void deselect() override;

    void deselect_text_field(bool input_complete = false);

    void add_char(char c);
    void display();

    void copy_to_clipboard();
    void paste_from_clipboard();

    void set_string(const char *str, bool to_fit_width = false);
    void set_number(const double number);

    void fit_width_to_string();

    void put_cursor_under_mouse(const Vec2d &mouse_pos);

    const char *get_string() const;
};


class TextEnterAcceptor : public EventAcceptor<v_TextField, Event::TextEnter> {
    friend v_TextField;

public:
    TextEnterAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::TextEnter &event, const EventAccResult *cur_res = nullptr) override;
};

class KeyDownTextFieldAcceptor : public EventAcceptor<v_TextField, Event::KeyDown> {
    friend v_TextField;

public:
    KeyDownTextFieldAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult *cur_res = nullptr) override;
};

class KeyUpTextFieldAcceptor : public EventAcceptor<v_TextField, Event::KeyUp> {
    friend v_TextField;

public:
    KeyUpTextFieldAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::KeyUp &event, const EventAccResult *cur_res = nullptr) override;
};

class TextFieldMousePressAcceptor : public EventAcceptor<v_TextField, Event::MousePress> {
    friend v_TextField;

public:
    TextFieldMousePressAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class TextFieldMouseReleaseAcceptor : public EventAcceptor<v_TextField, Event::MouseRelease> {
    friend v_TextField;

public:
    TextFieldMouseReleaseAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class TextFieldMouseMoveAcceptor : public EventAcceptor<v_TextField, Event::MouseMove> {
    friend v_TextField;

public:
    TextFieldMouseMoveAcceptor(v_TextField *acceptor);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class TextFieldChangeStringSynchronizer : public EventReaction<Event::TextChanged> {
    char **text;
public:
    TextFieldChangeStringSynchronizer(char **text_ptr);

    EventAccResult operator()(const Event::TextChanged &event, const EventAccResult*) override;
};

class TextFieldChangeValueSynchronizer : public EventReaction<Event::TextChanged> {
    double *val_d;
    int *val_i;
public:
    TextFieldChangeValueSynchronizer(double *val_d, int *val_i);
    TextFieldChangeValueSynchronizer(int *val_i);
    TextFieldChangeValueSynchronizer(double *val_d);

    EventAccResult operator()(const Event::TextChanged &event, const EventAccResult*) override;
};
