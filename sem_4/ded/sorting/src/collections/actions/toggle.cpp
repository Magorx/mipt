#include "toggle.h"




a_OnPressToggler::a_OnPressToggler(AbstractView *acceptor, v_Hideable *target, bool toggler, bool reversed) : 
EventAcceptor(acceptor),
target(target),
toggler(toggler),
reversed(reversed) {}

EventAccResult a_OnPressToggler::operator()(const Event::MousePress &event, const EventAccResult *) {
    AbstractView *av = acceptor;
    if (!av->is_inside(event.position)) return EventAccResult::none;

    if (toggler) target->toggle();
    else target->set_active(true ^ reversed);

    return EventAccResult::none;
}


a_OnReleaseToggler::a_OnReleaseToggler(AbstractView *acceptor, v_Hideable *target, bool toggler, bool reversed) : 
EventAcceptor(acceptor),
target(target),
toggler(toggler),
reversed(reversed)
{}

EventAccResult a_OnReleaseToggler::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    AbstractView *av = acceptor;
    if (!av->is_inside(event.position)) return EventAccResult::none;

    if (toggler) target->toggle();
    else target->set_active(false ^ reversed);

    return EventAccResult::none;
}


a_OnHoverToggler::a_OnHoverToggler(AbstractView *acceptor, v_Hideable *target, bool reversed) : EventAcceptor(acceptor), target(target), reversed(reversed) {
    acceptor->e_mouse_press.add(new a_OnPressToggler(acceptor, target, false, reversed));
}

EventAccResult a_OnHoverToggler::operator()(const Event::MouseMove &event, const EventAccResult *res) {
    AbstractView *av = acceptor;

    if ((av->is_inside(event.to) && !av->is_inside(event.from)) || (av->is_inside(event.to) && !target->is_active())) {
        target->set_active(true ^ reversed);
        return EventAccResult::cont;
    } else if (!av->is_inside(event.to) && av->is_inside(event.from)) {
        if (res && ((*res & EventAccResult::cont) || ((*res & EventAccResult::done) || ((*res & EventAccResult::stop))))) return EventAccResult::cont;

        target->set_active(false ^ reversed);
        return EventAccResult::cont;
    }

    return EventAccResult::none;
}

