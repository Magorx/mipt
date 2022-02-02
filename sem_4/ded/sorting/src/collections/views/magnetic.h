#pragma once


#include "highlighter.h"


extern const double PX_MAG_DOT;


class v_Magnetic;


class AVMagneticPressAcceptor : public EventAcceptor<v_Magnetic, Event::MousePress> {
    bool button_mag[(int)Event::MouseButton::MAX + 1];
public:
    AVMagneticPressAcceptor(v_Magnetic *magnetic);

    void set_button_mag(Event::MouseButton button, bool flag);

    void set_singular_mag_button(Event::MouseButton button);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};


class AVMagneticReleaseAcceptor : public EventAcceptor<v_Magnetic, Event::MouseRelease> {
public:
    AVMagneticReleaseAcceptor(v_Magnetic *magnetic);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};


class AVMagneticMoveAcceptor : public EventAcceptor<v_Magnetic, Event::MouseMove> {
public:
    AVMagneticMoveAcceptor(v_Magnetic *magnetic);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class AVMagneticScrollAcceptor : public EventAcceptor<v_Magnetic, Event::Scroll> {
    double granularity;
    bool to_consider_restriciton;
public:
    AVMagneticScrollAcceptor(v_Magnetic *magnetic, double granularity = 0.01, bool to_consider_restriciton = true);

    EventAccResult operator()(const Event::Scroll &event, const EventAccResult *cur_res = nullptr) override;
};


class v_Magnetic : public v_Highlighter {
    friend AVMagneticPressAcceptor;
    friend AVMagneticReleaseAcceptor;
    friend AVMagneticMoveAcceptor;
    friend AVMagneticScrollAcceptor;

    v_Highlighter *dot;

    bool to_be_pressed;
    double mag_radius;

    AVMagneticPressAcceptor *acc_press;

    bool x_shift_banned = false;
    bool y_shift_banned = false;

public:
    v_Magnetic(const ViewBody &bounds, const ViewBody &body = {PX_MAG_DOT, PX_MAG_DOT}, double mag_radius = NAN, bool press_respects_bound = true, bool to_be_pressed = true);

    virtual void render(Renderer *renderer) override;
    virtual void refit() override;

    v_Highlighter *get_dot() { return dot; }

    bool magnetize_to(const Vec2d &pos, bool to_check_mag_radius = true);
    inline bool magnetize_test(const Vec2d &pos) { return !(mag_radius == mag_radius && (pos - dot->get_body().position).len_squared() > mag_radius * mag_radius); }

    void update_bounds(const ViewBody &bounds_);

    void shift(const Vec2d &shift, bool with_bounds = true);
    void shift_bounds(const Vec2d &shift);

    Vec2d get_fraction() const;
    void set_fraction(Vec2d fraction);
    void shift_fraction(Vec2d shift);
    void shift_fraction(double shift);

    void toggle_x_restriction() { y_shift_banned ^= true; }
    void toggle_y_restriction() { x_shift_banned ^= true; }

    inline void emit_frac() {
        e_fraction_changed.emit({{body.size.x() ? dot->get_body().position.x() / body.size.x() : NAN,
                                  body.size.y() ? dot->get_body().position.y() / body.size.y() : NAN}});
    }

    inline Vec2d dot_coord() { return body.position + dot->get_body().position; }

    AVMagneticPressAcceptor *get_acc_press() { return acc_press; }
};
