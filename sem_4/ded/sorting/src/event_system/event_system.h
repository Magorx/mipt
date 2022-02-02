#pragma once


#include "abstract.h"
#include "event_types.h"


class EventSystem : public AbstractEventSystem<EventSystem> {
public:
    EventDispatcher<EventSystem, Event::Tick>            e_tick;

    EventDispatcher<EventSystem, Event::MousePress>      e_mouse_press;
    EventDispatcher<EventSystem, Event::MouseRelease>    e_mouse_release;
    EventDispatcher<EventSystem, Event::MouseMove>       e_mouse_move;
    EventDispatcher<EventSystem, Event::Scroll>          e_scroll;
    EventDispatcher<EventSystem, Event::MouseDrag>       e_mouse_drag;
    EventDispatcher<EventSystem, Event::Activator>       e_toggle;
    EventDispatcher<EventSystem, Event::ActivityToggle>  e_toggle_activity;
    EventDispatcher<EventSystem, Event::RenderCall>      e_render_call;
    EventDispatcher<EventSystem, Event::Close>           e_close;
    EventDispatcher<EventSystem, Event::Clicked>         e_clicked;
    EventDispatcher<EventSystem, Event::FractionChanged> e_fraction_changed;
    EventDispatcher<EventSystem, Event::VectorFractionChanged> e_vec_fraction_changed;

    EventDispatcher<EventSystem, Event::KeyDown>   e_key_down;
    EventDispatcher<EventSystem, Event::KeyUp>     e_key_up;
    EventDispatcher<EventSystem, Event::TextEnter> e_text_enter;

    EventDispatcher<EventSystem, Event::TextChanged> e_text_changed;
    EventDispatcher<EventSystem, Event::ColorChanged> e_color_changed;

    EventDispatcher<EventSystem, Event::DataPtr> e_data_ptr;

    EventSystem() :
    AbstractEventSystem(this),
    e_tick(this, "tick"),
    e_mouse_press(this, "mouse_press"),
    e_mouse_release(this, "mouse_release"),
    e_mouse_move(this, "mouse_move"),
    e_scroll(this, "scroll"),
    e_mouse_drag(this, "mouse_drag"),
    e_toggle(this, "toggle"),
    e_toggle_activity(this, "toggle_activity"),
    e_render_call(this, "render_call"),
    e_close(this, "close"),
    e_clicked(this, "clicked"),
    e_fraction_changed(this, "fraction_changed"),
    e_vec_fraction_changed(this, "vec_fraction_changed"),
    e_key_down(this, "key_down"),
    e_key_up(this, "key_up"),
    e_text_enter(this, "text_enter"),
    e_text_changed(this, "text_changed"),
    e_color_changed(this, "color_changed"),
    e_data_ptr(this, "data ptr")
    {}

    template <typename ES_T, typename T>
    EventDispatcher<ES_T, T> &get_dispatcher();
};

template <>
inline EventDispatcher<EventSystem, Event::MousePress> &EventSystem::get_dispatcher() {
    return e_mouse_press;
}

template <>
inline EventDispatcher<EventSystem, Event::MouseRelease> &EventSystem::get_dispatcher() {
    return e_mouse_release;
}

template <>
inline EventDispatcher<EventSystem, Event::MouseMove> &EventSystem::get_dispatcher() {
    return e_mouse_move;
}

template <>
inline EventDispatcher<EventSystem, Event::MouseDrag> &EventSystem::get_dispatcher() {
    return e_mouse_drag;
}

template <>
inline EventDispatcher<EventSystem, Event::Activator> &EventSystem::get_dispatcher() {
    return e_toggle;
}

template <>
inline EventDispatcher<EventSystem, Event::ActivityToggle> &EventSystem::get_dispatcher() {
    return e_toggle_activity;
}

template <>
inline EventDispatcher<EventSystem, Event::RenderCall> &EventSystem::get_dispatcher() {
    return e_render_call;
}

template <>
inline EventDispatcher<EventSystem, Event::Close> &EventSystem::get_dispatcher() {
    return e_close;
}

template <>
inline EventDispatcher<EventSystem, Event::Clicked> &EventSystem::get_dispatcher() {
    return e_clicked;
}

template <>
inline EventDispatcher<EventSystem, Event::FractionChanged> &EventSystem::get_dispatcher() {
    return e_fraction_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::VectorFractionChanged> &EventSystem::get_dispatcher() {
    return e_vec_fraction_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::DataPtr> &EventSystem::get_dispatcher() {
    return e_data_ptr;
}

template <>
inline EventDispatcher<EventSystem, Event::KeyDown> &EventSystem::get_dispatcher() {
    return e_key_down;
}

template <>
inline EventDispatcher<EventSystem, Event::KeyUp> &EventSystem::get_dispatcher() {
    return e_key_up;
}

template <>
inline EventDispatcher<EventSystem, Event::TextEnter> &EventSystem::get_dispatcher() {
    return e_text_enter;
}

template <>
inline EventDispatcher<EventSystem, Event::TextChanged> &EventSystem::get_dispatcher() {
    return e_text_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::ColorChanged> &EventSystem::get_dispatcher() {
    return e_color_changed;
}

template <>
inline EventDispatcher<EventSystem, Event::Scroll> &EventSystem::get_dispatcher() {
    return e_scroll;
}

template <>
inline EventDispatcher<EventSystem, Event::Tick> &EventSystem::get_dispatcher() {
    return e_tick;
}

#include "dispatcher_end.h"
