#pragma once


#include "highlighter.h"


class v_Stretcher : public v_Highlighter {
    int coord_idx;
    bool to_zerify_normal_coord = true;
public:

    enum Coord {
        x,
        y,
    };

    v_Stretcher(Coord coord_idx, const ViewBody &body, SmartColor *color = nullptr, AbstractView *parent = nullptr, double highlight_coef = 0.0) :
    v_Highlighter(body, color, parent, highlight_coef),
    coord_idx(coord_idx)
    {}

    static v_Stretcher *X(const ViewBody &body, SmartColor *color = nullptr, AbstractView *parent = nullptr, double highlight_coef = 0.0) {
        return new v_Stretcher(Coord::x, body, color, parent, highlight_coef);
    }

    static v_Stretcher *Y(const ViewBody &body, SmartColor *color = nullptr, AbstractView *parent = nullptr, double highlight_coef = 0.0) {
        return new v_Stretcher(Coord::y, body, color, parent, highlight_coef);
    }

    void set_zerify_normal_coord(bool to_zerify_normal_coord_) {
        to_zerify_normal_coord = to_zerify_normal_coord_;
    }

    void pre_add_subview(AbstractView *subview) {
        double sum_size = 0;
        for (auto subv : subviews) {
            sum_size += subv->get_body().size.content[coord_idx];
        }

        subview->get_body().position.content[coord_idx] = sum_size - subview->get_body().position.content[coord_idx];
        if (to_zerify_normal_coord) {
            subview->get_body().position.content[coord_idx ^ 1] = 0;
        }
        body.size.content[coord_idx] = sum_size;
    }

    virtual void add_subview(AbstractView *subview) override {
        pre_add_subview(subview);

        v_Highlighter::add_subview(subview);

        update();
    }

    void update() {
        double sum_size = 0;
        for (auto subv : subviews) {
            sum_size += subv->get_body().size.content[coord_idx];
        }
        body.size.content[coord_idx] = sum_size;
    }
    
    void add_placehodler(const double size) {
        AbstractView *av = new AbstractView({0, size});
        add_subview(av);
    }

    void normal_stretch(bool fit_max_size = false) {
        double max_size = body.size.content[coord_idx ^ 1];

        if (fit_max_size) {
            for (auto subv : subviews) {
                max_size = fmax(max_size, subv->get_body().size.content[coord_idx ^ 1]);
            }
        }

        for (auto subv : subviews) {
            subv->get_body().size.content[coord_idx ^ 1] = max_size;
            subv->recalculate_fit_body();
            subv->refit();
        }
    }
};
