#include "pbutton.h"
#include "collections/styles/button.h"
#include "collections/styles/text.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "redactor/engine.h"


FWD_ALL_FOR_CLASS_(WidgetButton)


class PButtonClicked : public EventAcceptor<WidgetButton, Event::Clicked> {
public:
    PButtonClicked(WidgetButton *widget) : EventAcceptor(widget) {}

    virtual EventAccResult operator()(const Event::Clicked &, const EventAccResult * = nullptr) override {
        auto button = acceptor->get_widget();
        auto &handler = button->get_handler();
        if (handler) {
            handler();
        }

        return EventAccResult::none;
    }
};


WidgetButton::WidgetButton(const ViewBody &body, PUPPY::Button *widget) :
v_Button(body, App.add_style(StdStyle::Button::blue())),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetButton);
    e_clicked.add(new PButtonClicked(this));
}

WidgetButton::WidgetButton(const Vec2d &pos, const char *caption, PUPPY::Button *widget) :
v_Button(caption, 
         App.add_style(StdStyle::Button::blue()),
         App.add_style(StdStyle::Text::basic()),
         PBUTTON_PADDING),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetButton);
    e_clicked.add(new PButtonClicked(this));

    body.position = pos;
}


PluginButton::PluginButton(const ViewBody &body, PUPPY::Widget *parent) :
PluginWidget(body, parent)
// PUPPY::Button(to_wbody(body), parent)
{
    view = new WidgetButton(body, this);
    set_body(to_wbody(view->get_body()));
    if (parent) {
        parent->add_child(this);
    }
}

PluginButton::PluginButton(const Vec2d &pos, const char *caption, PUPPY::Widget *parent) :
PluginWidget({pos, {0, 0}}, parent)
// PUPPY::Button(to_wbody(ViewBody{pos, 0}), parent)
{
    view = new WidgetButton(pos, caption, this);
    set_body(to_wbody(view->get_body()));
    if (parent) {
        parent->add_child(this);
    }
}
