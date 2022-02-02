#include "button.h"


const Vec2d BUTTON_HOVER_POS_DELTA = {0, -3};
const Vec2d BUTTON_CLICK_POS_DELTA = {0, 4};

double BUTTON_CLICKED_SHADING_COEF = 0.8;


v_Button::v_Button(const ViewBody &body, SmartColor *color, AbstractView *parent, double highlight_coef) :
v_Highlighter(body, color, parent, highlight_coef, true),
pos_delta(0, 0)
{
    e_mouse_press.add(new ButtonPressAcceptor(this));
    e_mouse_move.add(new ButtonMoveAcceptor(this));
    e_mouse_release.add(new ButtonReleaseAcceptor(this));

    covering_block = true;
}

v_Button::v_Button(const ViewBody &body, MouseReactionStyle *style, AbstractView *parent) :
v_Highlighter(body, nullptr, parent, 0, true),
pos_delta(0, 0),
style(style)
{
    appearenced = true;
    set_selectable(true);
    e_key_down.add(new AVSelectableFocuser(this));
    e_mouse_press.add(new AVSelectablePressDefocuser(this));
    e_key_down.add(new ButtonKeyDownAcceptor(this, Keyboard::Key::enter));
    e_key_up.add(new ButtonKeyUpAcceptor(this, Keyboard::Key::enter));

    if (style) {
        to_draw_selected_bounds = false;
        set_appearence(style->idle);
    }

    e_mouse_press.add(new ButtonPressAcceptor(this));
    e_mouse_release.add(new ButtonReleaseAcceptor(this));
    e_mouse_move.add(new ButtonMoveAcceptor(this));

    covering_block = true;
}

v_Button::v_Button(const char *label_name, MouseReactionStyle *style, TextStyle *label_style, const Vec2d &padding) :
v_Highlighter({0, Renderer::get_text_size(label_name, label_style->size, label_style->font) + padding * 2}, nullptr, nullptr, 0, true),
pos_delta(0, 0),
style(style)
{
    appearenced = true;
    set_selectable(true);
    e_key_down.add(new AVSelectableFocuser(this));
    e_mouse_press.add(new AVSelectablePressDefocuser(this));
    e_key_down.add(new ButtonKeyDownAcceptor(this, Keyboard::Key::enter));
    e_key_up.add(new ButtonKeyUpAcceptor(this, Keyboard::Key::enter));

    if (style) {
        to_draw_selected_bounds = false;
        set_appearence(style->idle);
    }

    e_mouse_press.add(new ButtonPressAcceptor(this));
    e_mouse_release.add(new ButtonReleaseAcceptor(this));
    e_mouse_move.add(new ButtonMoveAcceptor(this));

    add_label(label_name, label_style->size, label_style->foreground, label_style->background, true);

    covering_block = true;
}

v_Button::~v_Button() {
    // delete style;
}

void v_Button::render(Renderer *renderer) {
    // body.position += pos_delta;

    if (pressed && color) {
        color->set_rgb(color->rgb() * BUTTON_CLICKED_SHADING_COEF);
    }

    v_Highlighter::render(renderer);

    if (pressed && color) {
        color->set_rgb(color->rgb() / BUTTON_CLICKED_SHADING_COEF);
    }

    // body.position -= pos_delta;

    subrender(renderer);
}

void v_Button::press() {
    pressed = true;
    set_appearence(style->press);

    pos_delta += BUTTON_CLICK_POS_DELTA;
}

void v_Button::release() {
    pressed = false;
    set_appearence(style->release);

    pos_delta -= BUTTON_CLICK_POS_DELTA;
}

void v_Button::hover() {
    set_appearence(style->hover);
}

void v_Button::unhover() {
    set_appearence(style->unhover);
}

void v_Button::select(bool tabbed) {
    AbstractView::select(tabbed);

    if (pressed) release();
    hover();
}

void v_Button::deselect() {
    AbstractView::deselect();

    if (pressed) release();
    unhover();
}


ButtonPressAcceptor::ButtonPressAcceptor(v_Button *button, AVMouseReactionResources *res) : EventAcceptor(button), appr_presed(nullptr) {
    if (res) {
        appr_presed = new AppearenceTexture(res->pressed);
    }
}

EventAccResult ButtonPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    v_Button *button = acceptor;
    if (!button->is_inside(event.position)) return EventAccResult::stop;

    if (event.button == Event::MouseButton::left && !button->pressed) {
        button->press();
    }

    return EventAccResult::cont;
}


ButtonReleaseAcceptor::ButtonReleaseAcceptor(v_Button *button, AVMouseReactionResources *res) : EventAcceptor(button), appr_hovered(nullptr) {
    if (res) {
        appr_hovered = new AppearenceTexture(res->hovered);
    }
}

EventAccResult ButtonReleaseAcceptor::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    v_Button *button = acceptor;
    if (!button->is_inside(event.position)) return EventAccResult::stop;

    if (button->pressed) {
        button->release();
        button->e_clicked.emit({});
    }

    return EventAccResult::cont;
}


ButtonMoveAcceptor::ButtonMoveAcceptor(v_Button *button, AVMouseReactionResources *res) : EventAcceptor(button), appr_hovered(nullptr), appr_idle(nullptr) {
    if (res) {
        appr_hovered = new AppearenceTexture(res->hovered);
        appr_idle = new AppearenceTexture(res->idle);
    }
}

EventAccResult ButtonMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    v_Button *button = acceptor;
    if (!button->is_inside(event.to, event.from)) return EventAccResult::none;

    if (button->is_inside(event.to) && !button->is_inside(event.from)) {
        button->pos_delta += BUTTON_HOVER_POS_DELTA;
        acceptor->hover();
    }

    if (button->is_inside(event.from) && !button->is_inside(event.to)) {
        if (button->pressed) {
            button->release();
        }
        button->unhover();

        button->pos_delta -= BUTTON_HOVER_POS_DELTA;
    }

    return EventAccResult::cont;
}

ButtonKeyDownAcceptor::ButtonKeyDownAcceptor(v_Button *button, Keyboard::Key key) :
EventAcceptor(button),
key(key)
{}

EventAccResult ButtonKeyDownAcceptor::operator()(const Event::KeyDown &event, const EventAccResult *) {
    if (!acceptor->is_selected() || event.code != key) return EventAccResult::none;

    if (!acceptor->pressed) {
        acceptor->key_pressed = true;
        acceptor->press();
    }

    return EventAccResult::done;
}

ButtonKeyUpAcceptor::ButtonKeyUpAcceptor(v_Button *button, Keyboard::Key key) :
EventAcceptor(button),
key(key)
{}

EventAccResult ButtonKeyUpAcceptor::operator()(const Event::KeyUp &event, const EventAccResult *) {
    if (!acceptor->is_selected() || event.code != key) return EventAccResult::none;

    if (acceptor->key_pressed && acceptor->pressed) {
        acceptor->release();
        acceptor->e_clicked.emit({});
        acceptor->key_pressed = false;
    }

    return EventAccResult::done;
}
