#include "magnetic.h"


const double PX_MAG_DOT = 10;


v_Magnetic::v_Magnetic(const ViewBody &bounds_, const ViewBody &body, double mag_radius, bool press_respects_bound, bool to_be_pressed) :
v_Highlighter(bounds_),
dot(new v_Highlighter(body)),
to_be_pressed(to_be_pressed),
mag_radius(mag_radius)
{
    if (press_respects_bound) {
        e_mouse_press.add(new AVMissPressBlocker(this));
    }

    e_mouse_press.add(acc_press = new AVMagneticPressAcceptor(this));
    e_mouse_move.add(new AVMagneticMoveAcceptor(this));
    e_mouse_release.add(new AVMagneticReleaseAcceptor(this));
    e_scroll.add(new AVMagneticScrollAcceptor(this));

    add_subview(dot);
    auto appr = new AppearenceTexture(App.texture.dot);
    appr->set_screen_shift(-body.size/ 2);
    dot->set_appearence(App.add_appr(appr));

    covering_block = true;
}

void v_Magnetic::render(Renderer *renderer) {
    v_Highlighter::render(renderer);
}

void v_Magnetic::refit() {
    refit_in_parent();
    dot->recalculate_fit_body();
    v_Highlighter::refit();
}

void v_Magnetic::update_bounds(const ViewBody &bounds_) {
    body = {body.position, bounds_.size};
}

double clamp(double val, double l, double r) {
    return fmin(fmax(val, l), r);
}

bool v_Magnetic::magnetize_to(const Vec2d &pos, bool to_check_mag_radius) {
    if (to_check_mag_radius && mag_radius == mag_radius && (dot->get_body().position - pos).len() > mag_radius) return false;

    Vec2d prev_pos = dot->get_body().position;

    Vec2d allowed_pos = {
        clamp(pos[0], 0, body.size[0]),
        clamp(pos[1], 0, body.size[1]),
    };

    if (x_shift_banned) {
        allowed_pos.content[0] = prev_pos[0];
    }
    if (y_shift_banned) {
        allowed_pos.content[1] = prev_pos[1];
    }

    dot->get_body().position = allowed_pos;

    emit_frac();
    
    return true;
}

void v_Magnetic::shift(const Vec2d &shift, bool with_bounds) {
    body.position += shift;
    if (with_bounds) {
        body.position += shift;
    }
}

void v_Magnetic::shift_bounds(const Vec2d &shift) {
    body.position += shift;
}

Vec2d v_Magnetic::get_fraction() const { 
    return {body.size.x() ? dot->get_body().position.x() / body.size.x() : NAN,
            body.size.y() ? dot->get_body().position.y() / body.size.y() : NAN};
}

void v_Magnetic::set_fraction(Vec2d fraction) {
    fraction.content[0] = fmin(fmax(fraction.content[0], 0), 1);
    fraction.content[1] = fmin(fmax(fraction.content[1], 0), 1);

    dot->get_body().position = body.size * fraction;
    emit_frac();
}

void v_Magnetic::shift_fraction(Vec2d shift) {
    if (x_shift_banned) shift.content[0] = 0;
    if (y_shift_banned) shift.content[1] = 0;

    set_fraction(get_fraction() + shift);
}

void v_Magnetic::shift_fraction(double shift) {
    shift_fraction({shift, shift});
}

AVMagneticPressAcceptor::AVMagneticPressAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {
    memset(button_mag, 1, sizeof(button_mag));
}

EventAccResult AVMagneticPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!button_mag[(int)event.button]) return EventAccResult::none;

    acceptor->pressed = acceptor->magnetize_to(event.position);
    
    if (acceptor->pressed) {
        return (EventAccResult) (EventAccResult::done | EventAccResult::focus);
    } else {
        return EventAccResult::cont;
    }
}

void AVMagneticPressAcceptor::set_button_mag(Event::MouseButton button, bool flag) {
    button_mag[(int)button] = flag;
}

void AVMagneticPressAcceptor::set_singular_mag_button(Event::MouseButton button) {
    memset(button_mag, 0, sizeof(button_mag));
    set_button_mag(button, true);
}


AVMagneticReleaseAcceptor::AVMagneticReleaseAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticReleaseAcceptor::operator()(const Event::MouseRelease &, const EventAccResult *) {
    acceptor->pressed = false;

    return EventAccResult::cont;
}


AVMagneticMoveAcceptor::AVMagneticMoveAcceptor(v_Magnetic *magnetic) : EventAcceptor(magnetic) {}

EventAccResult AVMagneticMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (!acceptor->to_be_pressed || acceptor->is_pressed()) {
        acceptor->magnetize_to(event.to, false);
    }

    return EventAccResult::cont;
}

AVMagneticScrollAcceptor::AVMagneticScrollAcceptor(v_Magnetic *magnetic, double granularity, bool to_consider_restriciton) :
EventAcceptor(magnetic),
granularity(granularity),
to_consider_restriciton(to_consider_restriciton)
{}

EventAccResult AVMagneticScrollAcceptor::operator()(const Event::Scroll &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.position)) return EventAccResult::none;

    Vec2d delta = event.delta;
    if (to_consider_restriciton && acceptor->x_shift_banned && fabs(delta.x()) > fabs(delta.y())) {
        delta.content[1] = delta.content[0];
    }
    if (to_consider_restriciton && acceptor->y_shift_banned && fabs(delta.y()) > fabs(delta.x())) {
        delta.content[0] = delta.content[1];
    }

    acceptor->shift_fraction(delta * granularity);

    return EventAccResult::done;
}
