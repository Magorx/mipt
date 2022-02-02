#pragma once


#include "redactor/plugin_std/plugin_std.hpp"


class WidgetFactory : public PUPPY::WidgetFactory {
    virtual PUPPY::Button      *button       (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Button      *button       (const PUPPY::Vec2f &pos,  const char *capt, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Slider      *slider       (PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::TextField   *text_field   (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Window      *window       (const char *name, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::ColorPicker *color_picker (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Label       *label        (const PUPPY::Vec2f &pos, const char *text, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Widget      *abstract     (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
};
