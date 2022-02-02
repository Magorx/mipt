#include "widget_factory.h"

#include "widgets/pwidget.h"
#include "widgets/pwindow.h"
#include "widgets/pbutton.h"
#include "widgets/pslider.h"

#include "widgets/plabel.h"
#include "widgets/ptext_field.h"
#include "widgets/pcolor_picker.h"

PUPPY::Button *WidgetFactory::button(const PUPPY::WBody &body, PUPPY::Widget *parent) const {
    return new PluginButton(to_vbody(body), parent);
}

PUPPY::Button *WidgetFactory::button(const PUPPY::Vec2f &pos, const char *caption, PUPPY::Widget *parent) const {
    return new PluginButton({pos.x, pos.y}, caption, parent);
}

PUPPY::Slider *WidgetFactory::slider(PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget *parent) const {
    return new PluginSlider(type, to_vbody(body), parent);
}

PUPPY::TextField *WidgetFactory::text_field(const PUPPY::WBody &body, PUPPY::Widget *parent) const {
    return new PluginTextField(to_vbody(body), parent);
}

PUPPY::Window *WidgetFactory::window(const char *name, const PUPPY::WBody &body, PUPPY::Widget *parent) const {
    return new PluginWindow(name, to_vbody(body), parent);
}

PUPPY::ColorPicker *WidgetFactory::color_picker(const PUPPY::WBody &body, PUPPY::Widget *parent) const {
    return new PluginColorPicker(to_vbody(body), parent);
}

PUPPY::Label *WidgetFactory::label(const PUPPY::Vec2f &pos, const char *text, PUPPY::Widget *parent) const {
    return new PluginLabel(text, to_vbody(PUPPY::WBody{pos, {0, 0}}), parent);
}

PUPPY::Widget *WidgetFactory::abstract(const PUPPY::WBody &body, PUPPY::Widget *parent) const {
    return new PluginWidget(to_vbody(body), parent, true);
}
