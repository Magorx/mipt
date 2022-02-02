#pragma once


#include "pwidget.h"
#include "collections/redactor_views/color_picker.h"


class PluginColorPicker : public PluginWidget, public PUPPY::ColorPicker {
protected:
    PUPPY::ColorPicker::HandlerType handler;

public:
    PluginColorPicker(const ViewBody &body, PUPPY::Widget *parent = nullptr);

    PluginColorPicker(const Vec2d &pos, const char *caption, PUPPY::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }

    virtual PUPPY::RGBA get_color() override;
    virtual void set_color(PUPPY::RGBA color) override;
};

class WidgetColorPicker : public v_ColorPicker {
    PUPPY::ColorPicker *widget;

public:
    WidgetColorPicker(const ViewBody &body, PUPPY::ColorPicker *widget);

    WIDGET_FIX_(PUPPY::ColorPicker)
};
