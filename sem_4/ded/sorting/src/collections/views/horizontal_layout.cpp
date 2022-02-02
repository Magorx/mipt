#include "horizontal_layout.h"


v_HorizontalLayout::v_HorizontalLayout(ViewBody body, ViewBody padding_body, 
                                   double padding_split, 
                                   AbstractView *parent,
                                   SmartColor *color,
                                   double highlight_coef) :
AbstractLayoutView(body, padding_body, padding_split, parent, color, highlight_coef)
{}

v_HorizontalLayout::~v_HorizontalLayout() {
    
}

void v_HorizontalLayout::layout_refit() {
    // printf("+++++\n");
    int v_cnt = weights.size();
    if (v_cnt == 0) return;
    double overall_weight = sum_weight;

    int cur_min_x = fitting_body.position.x();
    double min_y = fitting_body.position.y();
    // printf("my y %g their %g\n", body.position.y(), min_y);
    double size_y = fitting_body.size.y();
    // printf("my size x %g their %g\n", body.size.y(), size_y);

    int fit_pixels = fitting_body.size.x() - padding_split * (v_cnt - 1);
    // printf("total fit pixels: %d for %g weight\n", fit_pixels, overall_weight);

    for (int i = 0; i < v_cnt - 1; ++i) {
        double w = weights[i];
        double coef = w / overall_weight;
        int x_pixels = fit_pixels * coef;

        if ((size_t) i >= subviews.size()) {
            return;
        }

        AbstractView *view = subviews[i];
        view->get_body().set_position({static_cast<double>(cur_min_x), min_y});
        view->get_body().set_size({static_cast<double>(x_pixels), size_y});
        view->recalculate_fit_body();
        view->refit();

        cur_min_x += x_pixels + padding_split;
        fit_pixels -= x_pixels;
        overall_weight -= w;
    }

    if (v_cnt - 1 >= (int) subviews.size()) {
        return;
    }

    AbstractView *view = subviews[v_cnt - 1];
    view->get_body().set_position({static_cast<double>(cur_min_x), min_y});
    view->get_body().set_size({static_cast<double>(fitting_body.size.x() - (double) cur_min_x), size_y});
    view->recalculate_fit_body();
    view->refit();
}

