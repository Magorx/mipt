#include "rgb_splined.h"
#include "engine/moga_engine.h"


const int RGB_MAPPING_CNT = 3;


eff_RGBSplined::eff_RGBSplined(Layer *layer) :
ShaderEffect(layer, App.shader.name.rgb_mapping)
{
    for (int i = 0; i < RGB_MAPPING_CNT; ++i) {
        mapping[i].resize(255);
        for (size_t j = 0; j < mapping[i].size(); ++j) {
            mapping[i][j] = (double) j / mapping[i].size();
        }
    }

    shader->setUniformArray("red",   &mapping[0][0], mapping[0].size());
    shader->setUniformArray("green", &mapping[1][0], mapping[1].size());
    shader->setUniformArray("blue",  &mapping[2][0], mapping[2].size());
    shader->setUniform("max_rgb", 254.0f);
}

void eff_RGBSplined::set_spline(int idx, AbstractView *view) {
    if (idx < 0 || idx > RGB_MAPPING_CNT || !view) return;

    view->e_vec_fraction_changed.add(new RGBMappingUpdate(this, idx));
}

void eff_RGBSplined::apply() {
    shader->setUniformArray("red",   &mapping[0][0], mapping[0].size());
    shader->setUniformArray("green", &mapping[1][0], mapping[1].size());
    shader->setUniformArray("blue",  &mapping[2][0], mapping[2].size());
    shader->setUniform("max_rgb", 254.0f);

    ShaderEffect::apply();
}


RGBMappingUpdate::RGBMappingUpdate(eff_RGBSplined *target, int idx) :
EventAcceptor(target),
idx(idx)
{
    if (idx >= RGB_MAPPING_CNT) {
        idx = RGB_MAPPING_CNT - 1;
    }
}

EventAccResult RGBMappingUpdate::operator()(const Event::VectorFractionChanged &event, const EventAccResult *) {
    for (size_t i = 0; i < event.data.size(); ++i) {
        float res = event.data[i] / event.max_coef;
        double map_idx = (double) i / event.data.size() * acceptor->mapping[0].size();
        acceptor->mapping[idx][(size_t) map_idx] = res;
    }

    acceptor->target->force_redraw();
    return EventAccResult::cont;
}


v_Window *eff_RGBSplined::create_settings_window(MogaEngine *engine) {
    v_Window *window = new v_Window("RGB mapping", {engine->random_screen_pos(), 300}, App.add_style(StdStyle::Window::basic()));
    engine->add_view(window);

    auto content = window->get_content();
    auto size = content->get_body().size;

    const double coef = 0.05;
    Vec2d sp_pos  = size * coef;
    Vec2d sp_size = size * (1 - coef * 2);

    v_Spline *spr = new v_Spline({sp_pos, sp_size}, {255, 40, 40}, {20, 20, 20, 50});
    v_Spline *spg = new v_Spline({sp_pos, sp_size}, {40, 255, 40}, {20, 20, 20, 50});
    v_Spline *spb = new v_Spline({sp_pos, sp_size}, {50, 50, 255}, {20, 20, 20, 50});
    
    window->add_subview(spr);
    window->add_subview(spg);
    window->add_subview(spb);

    set_spline(0, spr);
    set_spline(1, spg);
    set_spline(2, spb);

    window->get_header()->get_button_close()->set_to_delete(true, true);

    window->set_content_color({60, 60, 60});

    return window;
}
