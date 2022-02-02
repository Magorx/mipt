#pragma once


#include "pwidget.h"
#include "collections/views/magnetic.h"


constexpr double PSLIDER_DOT_PADDING = 4;


class PluginSlider : public PluginWidget, public PUPPY::Slider {
protected:
    PUPPY::Slider::HandlerType handler;
    PUPPY::Slider::Type type;

public:
    PluginSlider(PUPPY::Slider::Type type, const ViewBody &body, PUPPY::Widget *parent = nullptr);

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }

    virtual PUPPY::Vec2f get_fraction_2d() override;
    virtual void set_fraction_2d(const PUPPY::Vec2f &frac) override;

    virtual float get_fraction() override;
    virtual void set_fraction(float frac) override;
};

class WidgetSlider : public v_Magnetic {
    PUPPY::Slider *widget;

public:
    WidgetSlider(const ViewBody &body, PUPPY::Slider *widget);

    WIDGET_FIX_(PUPPY::Slider)
};
