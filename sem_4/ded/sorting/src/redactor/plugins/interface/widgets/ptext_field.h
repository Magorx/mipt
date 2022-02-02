#pragma once


#include "pwidget.h"
#include "collections/views/text_field.h"


class PluginTextField : public PluginWidget, public PUPPY::TextField {
protected:
    PUPPY::TextField::HandlerType handler;

public:
    PluginTextField(const ViewBody &body, PUPPY::Widget *parent = nullptr);

    PluginTextField(const Vec2d &pos, const char *caption, PUPPY::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }

    virtual std::string_view get_text() const override;
    virtual void set_text(std::string_view text) override;
};

class WidgetTextField : public v_TextField {
    PUPPY::TextField *widget;

public:
    WidgetTextField(const ViewBody &body, PUPPY::TextField *widget);

    WIDGET_FIX_(PUPPY::TextField)
};
