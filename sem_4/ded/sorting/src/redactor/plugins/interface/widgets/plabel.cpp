#include "plabel.h"
#include "collections/styles/text.h"
#include "utils/logger.h"
#include "engine/Resources.h"
#include "engine/moga_engine.h"


FWD_ALL_FOR_CLASS_(WidgetLabel)


WidgetLabel::WidgetLabel(const char *text, const ViewBody &body, PUPPY::Label *widget) :
v_Highlighter(body, nullptr, true),
widget(widget)
{
    INIT_FWD_TO_WIDGET(WidgetLabel);

    this->body.size = App.engine->visual->get_renderer()->get_text_size(text, App.font.size.basic);
    add_label(text, App.font.size.basic, App.font.color.basic_menu, 0, true);
}


PluginLabel::PluginLabel(const char *name, const ViewBody &body, PUPPY::Widget *parent) :
PluginWidget(body, parent)
// PUPPY::Label(to_wbody(body), parent)
{
    view = new WidgetLabel(name, body, this);
    set_body(to_wbody(view->get_body()));
    if (parent) {
        parent->add_child(this);
    }
}

void PluginLabel::set_text(std::string_view text) {
    auto label = view;

    label->get_body().size = App.engine->visual->get_renderer()->get_text_size(text.begin(), App.font.size.basic);
    label->add_label(text.begin(), App.font.size.basic, App.font.color.basic_menu);
}

std::string_view PluginLabel::get_text() const {
    auto label = view;

    return label->get_label_text();
}
