#include "toggler.h"


const double TOGGLER_PRESS_SHADOWER = 0.45;
const double TOGGLER_ONLINE_SHADOWER = 0.65;


v_Toggler::v_Toggler(const ViewBody &body, SmartColor *color_border, SmartColor *color_button, AbstractView *parent, bool online, float button_factor) :
v_Hideable(body, parent),
pressed(false),
online(online),
color_border(color_border),
color_button(color_button),
button_factor(button_factor)
{
    e_mouse_press.add(new TogglerPressAcceptor(this));
    e_mouse_release.add(new TogglerReleaseAcceptor(this));
    e_mouse_move.add(new TogglerMoveAcceptor(this));
}

void v_Toggler::render(Renderer *renderer) {
    renderer->draw_rectangle(body.position, body.size, color_border->rgb());

    if (!online) {
        color_button->set_rgb(color_button->rgb() * TOGGLER_ONLINE_SHADOWER);
    }
    if (pressed) {
        color_button->set_rgb(color_button->rgb() * TOGGLER_PRESS_SHADOWER);
    }

    renderer->draw_rectangle(body.position + body.size * (1 - button_factor) / 2, body.size * button_factor, color_button->rgb());

    if (!online) {
        color_button->set_rgb(color_button->rgb() / TOGGLER_ONLINE_SHADOWER);
    }
    if (pressed) {
        color_button->set_rgb(color_button->rgb() / TOGGLER_PRESS_SHADOWER);
    }
    
}

void v_Toggler::press() {
    pressed = true;
}

void v_Toggler::unpress() {
    pressed = false;
}


TogglerPressAcceptor::TogglerPressAcceptor(v_Toggler *button) : EventAcceptor(button) {}

EventAccResult TogglerPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.position)) return EventAccResult::stop;

    v_Toggler *togg = acceptor;

    togg->online = togg->online ^ true;
    togg->press();

    togg->e_toggle.emit({});

    return EventAccResult::cont;
}


TogglerReleaseAcceptor::TogglerReleaseAcceptor(v_Toggler *button) : EventAcceptor(button) {}

EventAccResult TogglerReleaseAcceptor::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.position)) return EventAccResult::stop;

    acceptor->unpress();

    return EventAccResult::cont;
}


TogglerMoveAcceptor::TogglerMoveAcceptor(v_Toggler *button) : EventAcceptor(button) {}

EventAccResult TogglerMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    v_Toggler *togg = acceptor;
    if (!togg->is_inside(event.to, event.from)) return EventAccResult::none;

    if (togg->is_inside(event.to) && !togg->is_inside(event.from)) {}

    if (togg->is_inside(event.from) && !togg->is_inside(event.to)) {
        if (togg->pressed) {
            togg->unpress();
        }
    }
    
    return EventAccResult::cont;
}
