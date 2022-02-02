#pragma once


#include "utils.h"
#include "collections/views/hideable.h"


class a_OnPressToggler : public EventAcceptor<AbstractView, Event::MousePress> {
    v_Hideable *target;

public:
    bool toggler;
    bool reversed;

    a_OnPressToggler(AbstractView *acceptor, v_Hideable *target, bool toggler = true, bool reversed = false);

    virtual EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};


class a_OnReleaseToggler : public EventAcceptor<AbstractView, Event::MouseRelease> {
    v_Hideable *target;

public:
    bool toggler;
    bool reversed;

    a_OnReleaseToggler(AbstractView *acceptor, v_Hideable *target, bool toggler = true,  bool reversed = false);

    virtual EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};


class a_OnHoverToggler : public EventAcceptor<AbstractView, Event::MouseMove> {
    v_Hideable *target;

public:
    bool reversed;
    a_OnHoverToggler(AbstractView *acceptor, v_Hideable *target, bool reversed = false);

    virtual EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};
