#include "pslider.h"
#include "collections/styles/window.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "engine/moga_engine.h"


FWD_ALL_FOR_CLASS_(WidgetSlider)


class PSliderFracChange : public EventAcceptor<WidgetSlider, Event::FractionChanged> {
public:
    PSliderFracChange(WidgetSlider *widget) : EventAcceptor(widget) {}

    virtual EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult * = nullptr) override {
        auto slider = acceptor->get_widget(); if (!slider) return EventAccResult::none;
        auto &handler = slider->get_handler();
        if (handler) {
            handler({(float) event.fraction.x(), (float) event.fraction.y()});
        }

        return EventAccResult::none;
    }
};


WidgetSlider::WidgetSlider(const ViewBody &body, PUPPY::Slider *widget) :
v_Magnetic(body),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetSlider);
    
    e_fraction_changed.add(new PSliderFracChange(this));

    App.engine->add_view(this);

}


PluginSlider::PluginSlider(PUPPY::Slider::Type type, const ViewBody &body, PUPPY::Widget *parent) :
PluginWidget(body, parent),
// PUPPY::Slider(to_wbody(body), parent),
type(type)
{
    auto slider = new WidgetSlider(body, this);
    view = slider;
    slider->set_appearence(App.add_appr(new AppearenceColor(App.color.slider.basic)));
    slider->set_fraction({0.5, 0.5});

    if (type == PUPPY::Slider::Type::X) {
        slider->toggle_x_restriction();

        slider->get_dot()->get_body().size.content[1] = (slider->get_body().size - PSLIDER_DOT_PADDING).y();
        slider->get_dot()->set_appearence(App.add_appr(new AppearenceTexture(App.texture.stick_vert, {1, 1}, -slider->get_dot()->get_body().size / 2)));
    } else if (type == PUPPY::Slider::Type::Y) {
        slider->toggle_y_restriction();

        slider->get_dot()->get_body().size.content[0] = (slider->get_body().size - PSLIDER_DOT_PADDING).x();
        slider->get_dot()->set_appearence(App.add_appr(new AppearenceTexture(App.texture.stick_horz, {1, 1}, -slider->get_dot()->get_body().size / 2)));
    }

    set_body(to_wbody(view->get_body()));

    if (parent) {
        parent->add_child(this);
    }
}

PUPPY::Vec2f PluginSlider::get_fraction_2d() {
    auto slider = dynamic_cast<v_Magnetic*>(view); if (!slider) return {0, 0};

    auto frac = slider->get_fraction();
    return {(float) frac.x(), (float) frac.y()};
}

void PluginSlider::set_fraction_2d(const PUPPY::Vec2f &frac) {
    auto slider = dynamic_cast<v_Magnetic*>(view); if (!slider) return;

    slider->set_fraction({frac.x, frac.y});
}


float PluginSlider::get_fraction() {
    auto slider = dynamic_cast<v_Magnetic*>(view); if (!slider) return 0;
    
    auto frac = slider->get_fraction();
    if (type == PUPPY::Slider::Type::X) {
        return (float) frac.x();
    } else if (type == PUPPY::Slider::Type::Y) {
        return (float) frac.y();
    } else {
        return (float) fmax(frac.x(), frac.y());
    }
}

void PluginSlider::set_fraction(float frac) {
    auto slider = dynamic_cast<v_Magnetic*>(view); if (!slider) return;

    slider->shift_fraction(Vec2d{frac, frac} - slider->get_fraction());

    // if (type == PUPPY::Slider::Type::X) {
    //     return (float) frac.x();
    // } else if (type == PUPPY::Slider::Type::Y) {
    //     return (float) frac.y();
    // } else {
    //     return (float) fmax(frac.x(), frac.y());
    // }
    
    // slider->set_fraction({frac, frac});
}

