#include "ptext_field.h"
#include "collections/styles/text.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "engine/moga_engine.h"


FWD_ALL_FOR_CLASS_(WidgetTextField)


class PTextFieldChanged : public EventAcceptor<WidgetTextField, Event::TextChanged> {
public:
    PTextFieldChanged(WidgetTextField *widget) : EventAcceptor(widget) {}

    virtual EventAccResult operator()(const Event::TextChanged &, const EventAccResult * = nullptr) override {
        auto field = acceptor->get_widget();
        auto &handler = field->get_handler();
        if (handler) {
            handler(field->get_text());
        }

        return EventAccResult::none;
    }
};


WidgetTextField::WidgetTextField(const ViewBody &body, PUPPY::TextField *widget) :
v_TextField(
    body,
    App.add_style(StdStyle::Text::basic()),
    App.color.text_field.basic.frame,
    App.color.text_field.basic.content,
    false,
    true,
    true // autofit text size to height
),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetTextField);
    e_text_changed.add(new PTextFieldChanged(this));
}


PluginTextField::PluginTextField(const ViewBody &body, PUPPY::Widget *parent) :
PluginWidget(body, parent)
// PUPPY::TextField(to_wbody(body), parent)
{
    view = new WidgetTextField(body, this);
    if (parent) {
        parent->add_child(this);
    }
}

std::string_view PluginTextField::get_text() const {
    auto field = dynamic_cast<v_TextField*>(view); if (!field) return "";

    return field->get_string();
}

void PluginTextField::set_text(std::string_view text) {
    auto field = dynamic_cast<v_TextField*>(view); if (!field) return;

    field->set_string(text.begin());
}
