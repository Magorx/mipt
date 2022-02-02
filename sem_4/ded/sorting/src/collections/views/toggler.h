#pragma once


#include "hideable.h"
#include "../renderables/renderables.h"


extern const double TOGGLER_PRESS_SHADOWER;
extern const double TOGGLER_ONLINE_SHADOWER;


class v_Toggler;


class TogglerPressAcceptor : public EventAcceptor<v_Toggler, Event::MousePress> {
public:
    TogglerPressAcceptor(v_Toggler *button);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class TogglerReleaseAcceptor : public EventAcceptor<v_Toggler, Event::MouseRelease> {
public:
    TogglerReleaseAcceptor(v_Toggler *button);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class TogglerMoveAcceptor : public EventAcceptor<v_Toggler, Event::MouseMove> {
public:
    TogglerMoveAcceptor(v_Toggler *button);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class v_Toggler : public v_Hideable {
    bool pressed;
    bool online;

    SmartColor *color_border;
    SmartColor *color_button;

    float button_factor;

    friend TogglerPressAcceptor;
    friend TogglerReleaseAcceptor;
    friend TogglerMoveAcceptor;

public:
    v_Toggler(const ViewBody &body, SmartColor *color_border, SmartColor *color_button, AbstractView *parent = nullptr, bool online = false, float button_factor = 0.7);

    virtual void render(Renderer *renderer) override;

    void press();
    void unpress();
};
