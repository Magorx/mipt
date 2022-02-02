#pragma once


#include "highlighter.h"


class AbstractLayoutView : public v_Highlighter {
protected:
    std::vector<double> weights;
    double sum_weight;

    ViewBody fitting_body_scale;
    ViewBody fitting_body;
    double padding_split;

    virtual void layout_refit() {}

public:
    AbstractLayoutView(ViewBody body, ViewBody fitting_body_scale_ = {{0, 0}, {0, 0}}, double padding_split = 0, AbstractView *parent = nullptr, SmartColor *color = nullptr, double highlight_coef = 0):
    v_Highlighter(body, color, parent, highlight_coef),
    sum_weight(0),
    fitting_body_scale(fitting_body_scale_),
    fitting_body(fitting_body_scale),
    padding_split(padding_split)
    {
        if (fitting_body_scale.position.len_squared() < 0.001 && fitting_body_scale.size.len_squared() < 0.001) {
            fitting_body_scale = {{0, 0}, {1, 1}};
        }

        fitting_body.position = body.size * fitting_body_scale.position;
        fitting_body.size     = body.size * fitting_body_scale.size - fitting_body.position;
    }

    virtual ~AbstractLayoutView() {}

    virtual void recalculate_fit_body() override {
        AbstractView::recalculate_fit_body();

        fitting_body.position = body.size * fitting_body_scale.position;
        if (fitting_body.position.len_squared() < 2) {
            fitting_body.position = 0;
        }
        fitting_body.size = body.size * fitting_body_scale.size;
    }

    virtual void refit() override {
        AbstractView::refit();
        recalculate_fit_body();
        layout_refit();
    }

    inline void layout_add(AbstractView *view, double weight = 1) {
        if (weight < 0) return;

        add_subview(view);
        sum_weight += weight;
        weights.push_back(weight);

        layout_refit();
    }

    inline AbstractView *add_spaceholder(double weight = 1) {
        if (weight < 0) return nullptr;

        AbstractView *view = new v_Highlighter({{0, 0}, {0, 0}});
        add_subview(view);
        sum_weight += weight;
        weights.push_back(weight);

        layout_refit();

        return view;
    }
};