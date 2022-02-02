#include "pwindow.h"
#include "collections/styles/window.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "engine/moga_engine.h"


FWD_ALL_FOR_CLASS_(WidgetWindow)


class PWindowActivity : public EventAcceptor<WidgetWindow, Event::ActivityToggle> {
public:
    PWindowActivity(WidgetWindow *widget) : EventAcceptor(widget) {}

    virtual EventAccResult operator()(const Event::ActivityToggle &event, const EventAccResult * = nullptr) override {
        auto window = acceptor->get_widget();
        auto &handler = (event.mode == event.on) ? window->get_show_handler() : window->get_hide_handler();
        if (handler) {
            handler();
        }

        return EventAccResult::none;
    }
};


WidgetWindow::WidgetWindow(const char *name, const ViewBody &body, PUPPY::Window *widget) :
v_Window(name, body),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetWindow);

    this->set_active(false);
    get_header()->get_button_close()->set_active(false);
    
    e_toggle_activity.add(new PWindowActivity(this));

    App.engine->add_view(this);
}


PluginWindow::PluginWindow(const char *name, const ViewBody &body, PUPPY::Widget *parent) :
PluginWidget(body, parent)
// PUPPY::Window(to_wbody(body), parent)
{
    view = new WidgetWindow(name, body, this);
    if (parent) {
        parent->add_child(this);
    }
}

bool PluginWindow::set_name(const char *name) {
    auto window = dynamic_cast<v_Window*>(view);
    if (!window) return false;

    window->set_name(name);
    return true;
}

const char *PluginWindow::get_name() {
    auto window = dynamic_cast<v_Window*>(view);
    if (!window) return nullptr;

    return window->get_name();
}

void PluginWindow::hide() {
    view->set_active(false);
}

void PluginWindow::show() {
    view->set_active(true);
    auto window = dynamic_cast<v_Window*>(view);
    if (!window) return;

    return window->focus();
}
