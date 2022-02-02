#pragma once


#include "highlighter.h"
#include "visual/style/mouse_reaction_style.h"
#include "visual/style/text_style.h"


extern const Vec2d BUTTON_HOVER_POS_DELTA;
extern const Vec2d BUTTON_CLICK_POS_DELTA;
extern double BUTTON_CLICKED_SHADING_COEF;


class v_Button;


class ButtonPressAcceptor : public EventAcceptor<v_Button, Event::MousePress> {
    friend v_Button;

    AppearenceTexture *appr_presed;
public:
    ButtonPressAcceptor(v_Button *button, AVMouseReactionResources *res = nullptr);
    virtual ~ButtonPressAcceptor() { delete appr_presed; }

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class ButtonReleaseAcceptor : public EventAcceptor<v_Button, Event::MouseRelease> {
    friend v_Button;

    AppearenceTexture *appr_hovered;
public:
    ButtonReleaseAcceptor(v_Button *button, AVMouseReactionResources *res = nullptr);
    virtual ~ButtonReleaseAcceptor() { delete appr_hovered; }

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class ButtonMoveAcceptor : public EventAcceptor<v_Button, Event::MouseMove> {
    friend v_Button;

    AppearenceTexture *appr_hovered;
    AppearenceTexture *appr_idle;
public:
    ButtonMoveAcceptor(v_Button *button, AVMouseReactionResources *res = nullptr);
    virtual ~ButtonMoveAcceptor() { delete appr_hovered; delete appr_idle; }

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class ButtonKeyDownAcceptor : public EventAcceptor<v_Button, Event::KeyDown> {
    Keyboard::Key key;
public:
    ButtonKeyDownAcceptor(v_Button *button, Keyboard::Key key);

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult *cur_res = nullptr) override;
};

class ButtonKeyUpAcceptor : public EventAcceptor<v_Button, Event::KeyUp> {
    Keyboard::Key key;
public:
    ButtonKeyUpAcceptor(v_Button *button, Keyboard::Key key);

    EventAccResult operator()(const Event::KeyUp &event, const EventAccResult *cur_res = nullptr) override;
};


class v_Button : public v_Highlighter {
    Vec2d pos_delta;

    MouseReactionStyle *style;

    friend ButtonPressAcceptor;
    friend ButtonReleaseAcceptor;
    friend ButtonMoveAcceptor;
    friend ButtonKeyDownAcceptor;
    friend ButtonKeyUpAcceptor;

    bool key_pressed;

public:
    v_Button(const ViewBody &body, SmartColor *color = nullptr, AbstractView *parent = nullptr, double highlight_coef = HIGHLIGHTER_ON_COEF);
    v_Button(const ViewBody &body, MouseReactionStyle *style, AbstractView *parent = nullptr);
    v_Button(const char *label_name, MouseReactionStyle *style, TextStyle *label_style, const Vec2d &padding = {0, 0});

    virtual ~v_Button();

    virtual void render(Renderer *renderer) override;

    virtual void select(bool tabbed = false) override;
    virtual void deselect() override;

    void press();
    void release();
    void hover();
    void unhover();
};
