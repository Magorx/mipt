#pragma once


#include "../../view/view.h"


class v_Hideable;


class HideablePressAcceptor : public EventAcceptor<v_Hideable, Event::MousePress> {
public:
    HideablePressAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class HideableReleaseAcceptor : public EventAcceptor<v_Hideable, Event::MouseRelease> {
public:
    HideableReleaseAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class HideableMoveAcceptor : public EventAcceptor<v_Hideable, Event::MouseMove> {
public:
    HideableMoveAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class HideableActivityToggleAcceptor : public EventAcceptor<v_Hideable, Event::ActivityToggle> {
public:
    HideableActivityToggleAcceptor(v_Hideable *button);

    EventAccResult operator()(const Event::ActivityToggle &event, const EventAccResult *cur_res = nullptr) override;
};


class v_Hideable : public AbstractView {
    friend HideablePressAcceptor;
    friend HideableReleaseAcceptor;
    friend HideableMoveAcceptor;
    friend HideableActivityToggleAcceptor;

    bool _is_active;

public:
    bool to_pass_inactive;

    v_Hideable(const ViewBody &body, AbstractView *parent = nullptr, bool to_pass_inactive = false, bool is_shown = true, bool to_block_covered = false);

    virtual void render(Renderer *renderer) override;

    inline void deactivate() { _is_active  = false; e_toggle_activity.dispatch_to_sub_es({Event::Activator::State::off}); }
    inline void activate()   { _is_active  = true;  }
    inline void toggle()     { _is_active ^= true;  }

    virtual void set_active(bool flag) { _is_active = flag; }

    void become_toggle_reacting();

    virtual bool is_active() const override final { return _is_active; }
};
