#pragma once


#include "pwidget.h"
#include "collections/views/button.h"


constexpr double PBUTTON_PADDING = 4;


class PluginButton : public PluginWidget, public PUPPY::Button {
protected:
    PUPPY::Button::HandlerType handler;

public:
    PluginButton(const ViewBody &body, PUPPY::Widget *parent = nullptr);

    PluginButton(const Vec2d &pos, const char *caption, PUPPY::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }
};

class WidgetButton : public v_Button {
    PUPPY::Button *widget;

public:
    WidgetButton(const ViewBody &body, PUPPY::Button *widget);
    WidgetButton(const Vec2d &pos, const char *caption, PUPPY::Button *widget);

    WIDGET_FIX_(PUPPY::Button)
};
