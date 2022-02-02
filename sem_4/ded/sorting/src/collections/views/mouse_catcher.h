#pragma once


#include "highlighter.h"


class v_MouseCatcher;


class MouseCatcherPressAcceptor : public EventAcceptor<v_MouseCatcher, Event::MousePress> {
public:
    MouseCatcherPressAcceptor(v_MouseCatcher *mouse_catcher);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class MouseCatcherMoveAcceptor : public EventAcceptor<v_MouseCatcher, Event::MouseMove> {
public:
    MouseCatcherMoveAcceptor(v_MouseCatcher *mouse_catcher);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};


class v_MouseCatcher : public v_Highlighter {
    Vec2d pos_delta;

    bool captured;

    friend MouseCatcherPressAcceptor;
    friend MouseCatcherMoveAcceptor;

public:
    v_MouseCatcher(const ViewBody &body, AbstractView *parent = nullptr, SmartColor *color=nullptr);
    virtual ~v_MouseCatcher();

    void press(Vec2d click);
    void move(Vec2d from, Vec2d to);
    void released(Vec2d click);

    void capture();
    void uncapture();

    inline void toggle() { if (captured) uncapture(); else capture(); }
};
