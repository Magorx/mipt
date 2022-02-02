#include "color_picker.h"


const double PX_COLOR_PICKER_PADDING = 0;
const double PX_COLOR_PICKER_CIRCLE_SIZE = 10;


v_ColorPicker::v_ColorPicker(const ViewBody &body, ToolManager *tool_manager):
v_Highlighter(body),

tool_manager(tool_manager),

v_vert_layout(new v_VerticalLayout(
    {PX_COLOR_PICKER_PADDING, body.size - 2 * PX_COLOR_PICKER_PADDING},
    {0, 1}, 
    0)
),
v_horz_layout(new v_HorizontalLayout(
    {0, 0},
    {0, 1},
    0)
),

appr_field(new AppearenceTexture(nullptr)),
appr_spectrum(new AppearenceTexture(App.color.spectrum)),
appr_alpha(new AppearenceTexture(nullptr)),

v_field(new v_Magnetic({0, 0}, {0, PX_COLOR_PICKER_CIRCLE_SIZE})),
v_spectrum(new v_Magnetic({0, 0}, {0, PX_COLOR_PICKER_CIRCLE_SIZE})),
v_alpha(new v_Magnetic({0, 0}, {0, PX_COLOR_PICKER_CIRCLE_SIZE})),

v_transparency(new v_Highlighter({0, body.size})),

field_size(v_field->get_body().size),
field(nullptr),
field_texture(nullptr),

alpha_size(v_alpha->get_body().size),
alpha(nullptr),
alpha_texture(nullptr)

{
    add_subview(v_transparency);
    
    add_subview(v_vert_layout);

    v_vert_layout->add_spaceholder(0.35)->set_appearence(App.add_appr(new AppearenceColor(App.texture.window.color.basic_frame)));
    v_vert_layout->layout_add(v_field, 8);
    v_vert_layout->add_spaceholder(0.35)->set_appearence(App.add_appr(new AppearenceColor(App.texture.window.color.basic_frame)));
    v_vert_layout->layout_add(v_spectrum, 1);
    v_vert_layout->layout_add(v_alpha, 1);

    v_field->focus();

    v_field->set_appearence(appr_field);
    v_spectrum->set_appearence(appr_spectrum);
    v_alpha->set_appearence(appr_alpha);

    v_spectrum->toggle_x_restriction();
    v_alpha->toggle_x_restriction();

    v_transparency->get_body() = v_alpha->get_body();
    v_transparency->get_body().position += v_vert_layout->get_body().position;

    // scaling squares to fit right in

    auto appr = new AppearenceTexture(App.texture.transparency_squares, v_transparency->get_body().size, true);
    auto _trs_size = App.texture.transparency_squares->getSize();
    Vec2d trs_size = {(double) _trs_size.x, (double) _trs_size.y};
    double scale_y = trs_size.y() / v_transparency->get_body().size.y();
    appr->set_transform(appr->transform * scale_y);
    v_transparency->set_appearence(App.add_appr(appr));

    //  ======================================================================= Geometry is done

    field = new RColor[field_size.x() * field_size.y()];
    field_texture = new RTexture;
    field_texture->create(field_size.x(), field_size.y());
    appr_field->get_render_mode()->texture = field_texture;

    alpha = new RColor[alpha_size.x() * alpha_size.y()];
    alpha_texture = new RTexture;
    alpha_texture->create(alpha_size.x(), alpha_size.y());
    appr_alpha->get_render_mode()->texture = alpha_texture;

    v_spectrum->e_fraction_changed.add(new ColorPickerSpectrumChangeAcceptor(this));
    v_field->e_fraction_changed.add(new ColorPickerFieldChangeAcceptor(this));
    v_alpha->e_fraction_changed.add(new ColorPickerAlphaChangeAcceptor(this));

    appr_spectrum->set_transform({1, -0.01});
    appr_spectrum->update_image(); // musthave
    
    appr_alpha->update_image();

    update_colors();

    v_field->set_fraction({0, 1});
    v_spectrum->set_fraction({0, 0.5});
    v_alpha->set_fraction({1, 0.5});

    covering_block = true;
}

// void v_ColorPicker::render(Renderer *renderer) {
//     v_Highlighter::render(renderer);
// }

void v_ColorPicker::update_colors() {
    update_field();
    update_alpha();
    update_tool_manager_color();
}

void v_ColorPicker::update_field() {
    Vec2d px_coord = v_spectrum->get_fraction() * v_spectrum->get_body().size;
    RColor base_color = v_spectrum->get_appearence()->get_px_color(px_coord);

    for (double y = 0; y < field_size.y(); y += 1) {
        if ((int) y >= (int) field_size.y()) break;
        for (double x = 0; x < field_size.x(); x += 1) {
            if ((int) x >= (int) field_size.x()) break;

            Vec2d frac = {x / (field_size.x() - 1), 1 - y / (field_size.y() - 1)};

            Color color = Color(base_color.r, base_color.g, base_color.b, 255);
            color *= frac.x();
            color += Color{255, 255, 255, 255} * (1.0 - frac.x());

            color *= frac.y();

            RColor final_color = color;
            final_color.a = base_color.a;
            field[(int)(y * field_size.x() + x)] = final_color;
        }
    }

    field_texture->update((sf::Uint8*) field);
    appr_field->update_image();
}

void v_ColorPicker::update_alpha() {
    Vec2d px_coord = v_field->get_fraction() * v_field->get_body().size;
    RColor base_color = v_field->get_appearence()->get_px_color(px_coord);

    for (double y = 0; y < alpha_size.y(); y += 1) {
        if ((int) y >= (int) alpha_size.y()) break;
        for (double x = 0; x < alpha_size.x(); x += 1) {
            if ((int) x >= (int) alpha_size.x()) break;

            Vec2d frac = {x / (alpha_size.x() - 1), 1 - y / (alpha_size.y() - 1)};

            double alpha_coef = frac.x();
            Color color = Color(base_color.r, base_color.g, base_color.b, 255 * alpha_coef);

            RColor final_color = color;
            alpha[(int)(y * alpha_size.x() + x)] = final_color;
        }
    }

    alpha_texture->update((sf::Uint8*) alpha);
    appr_alpha->update_image();
}

void v_ColorPicker::update_tool_manager_color(RColor *force_color) {
    if (force_color) {
        cur_color = *force_color;
        if (tool_manager) tool_manager->set_draw_color(*force_color);
        e_color_changed.emit({cur_color});
        return;
    }

    Vec2d px_coord = v_field->get_fraction() * v_field->get_body().size;
    RColor color = v_field->get_appearence()->get_px_color(px_coord);

    px_coord = v_alpha->get_fraction() * v_alpha->get_body().size;
    color.a = v_alpha->get_appearence()->get_px_color(px_coord).a;

    cur_color = color;

    if (tool_manager) tool_manager->set_draw_color(cur_color);
    e_color_changed.emit({cur_color});
}

v_ColorPicker::~v_ColorPicker() {
    delete appr_field;
    delete appr_spectrum;
    delete field_texture;

    delete[] field;
    delete[] alpha;
};

RColor v_ColorPicker::get_color() {
    return cur_color;
}

void v_ColorPicker::set_color(RColor color) {
    cur_color = color;
    update_tool_manager_color(&cur_color);
}


ColorPickerSpectrumChangeAcceptor::ColorPickerSpectrumChangeAcceptor(v_ColorPicker *color_picker) : 
EventAcceptor(color_picker)
{}

EventAccResult ColorPickerSpectrumChangeAcceptor::operator()(const Event::FractionChanged &, const EventAccResult *) {
    acceptor->update_colors();

    return EventAccResult::cont;
}

ColorPickerFieldChangeAcceptor::ColorPickerFieldChangeAcceptor(v_ColorPicker *color_picker) : 
EventAcceptor(color_picker)
{}

EventAccResult ColorPickerFieldChangeAcceptor::operator()(const Event::FractionChanged &, const EventAccResult *) {
    acceptor->update_alpha();
    acceptor->update_tool_manager_color();

    return EventAccResult::cont;
}

ColorPickerAlphaChangeAcceptor::ColorPickerAlphaChangeAcceptor(v_ColorPicker *color_picker) : 
EventAcceptor(color_picker)
{}

EventAccResult ColorPickerAlphaChangeAcceptor::operator()(const Event::FractionChanged &, const EventAccResult *) {
    acceptor->update_tool_manager_color();

    return EventAccResult::cont;
}
