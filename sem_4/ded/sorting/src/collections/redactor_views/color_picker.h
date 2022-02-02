#pragma once


#include "utils.h"


extern const double PX_COLOR_PICKER_PADDING;
extern const double PX_COLOR_PICKER_CIRCLE_SIZE;


class ColorPickerSpectrumChangeAcceptor;
class ColorPickerFieldChangeAcceptor;
class ColorPickerAlphaChangeAcceptor;


class v_ColorPicker : public v_Highlighter {
    friend ColorPickerSpectrumChangeAcceptor;
    friend ColorPickerFieldChangeAcceptor;
    friend ColorPickerAlphaChangeAcceptor;

    ToolManager *tool_manager;

    v_VerticalLayout *v_vert_layout;
    v_HorizontalLayout *v_horz_layout;

    AppearenceTexture *appr_field;
    AppearenceTexture *appr_spectrum;
    AppearenceTexture *appr_alpha;

    v_Magnetic *v_field;
    v_Magnetic *v_spectrum;
    v_Magnetic *v_alpha;

    v_Highlighter *v_transparency;


    Vec2d &field_size;
    RColor *field;
    RTexture *field_texture;

    Vec2d &alpha_size;
    RColor *alpha;
    RTexture *alpha_texture;
    RColor cur_color;

    void update_colors();

    void update_field();
    void update_alpha();
    void update_tool_manager_color(RColor *force_color = nullptr);

public:
    v_ColorPicker(const ViewBody &body, ToolManager *tool_manager);
    virtual ~v_ColorPicker();

    RColor get_color();
    void set_color(RColor color);
};

class ColorPickerSpectrumChangeAcceptor : public EventAcceptor<v_ColorPicker, Event::FractionChanged> {
    friend v_ColorPicker;
public:
    ColorPickerSpectrumChangeAcceptor(v_ColorPicker *color_picker);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};

class ColorPickerFieldChangeAcceptor : public EventAcceptor<v_ColorPicker, Event::FractionChanged> {
    friend v_ColorPicker;
public:
    ColorPickerFieldChangeAcceptor(v_ColorPicker *color_picker);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};

class ColorPickerAlphaChangeAcceptor : public EventAcceptor<v_ColorPicker, Event::FractionChanged> {
    friend v_ColorPicker;
public:
    ColorPickerAlphaChangeAcceptor(v_ColorPicker *color_picker);

    EventAccResult operator()(const Event::FractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};
