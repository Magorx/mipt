#pragma once


#include <vector>

#include "utils.h"
#include "redactor/layer/layer.h"
#include "redactor/canvas/canvas.h"

#include "view/view.h"
#include "collections/views/spline.h"

#include "event_system/event_reaction.h"


extern const int RGB_MAPPING_CNT;


class RGBMappingUpdate;


class eff_RGBSplined : public ShaderEffect, public WindowSetter {
    friend RGBMappingUpdate;

    std::vector<float> mapping[3];

public:
    eff_RGBSplined(Layer *layer);

    void set_spline(int idx, AbstractView *view);

    virtual void apply() override;

    virtual v_Window *create_settings_window(MogaEngine *engine) override;
};


class RGBMappingUpdate : public EventAcceptor<eff_RGBSplined, Event::VectorFractionChanged> {
    int idx;

public:
    RGBMappingUpdate(eff_RGBSplined *target, int idx);

    virtual EventAccResult operator()(const Event::VectorFractionChanged &event, const EventAccResult *cur_res = nullptr) override;
};
