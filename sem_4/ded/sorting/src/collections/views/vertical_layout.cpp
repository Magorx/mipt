#include "vertical_layout.h"


v_VerticalLayout::v_VerticalLayout(ViewBody body, ViewBody padding_body, 
                                   double padding_split, 
                                   AbstractView *parent,
                                   SmartColor *color,
                                   double highlight_coef) :
AbstractLayoutView(body, padding_body, padding_split, parent, color, highlight_coef)
{}

v_VerticalLayout::~v_VerticalLayout() {
    
}

void v_VerticalLayout::layout_refit() {
    // printf("-----\n");
    int v_cnt = weights.size();
    if (v_cnt == 0) return;
    double overall_weight = sum_weight;

    int cur_min_y = fitting_body.position.y();
    double min_x = fitting_body.position.x();
    // printf("my x %g their %g\n", body.position.x(), min_x);
    double size_x = fitting_body.size.x();
    // printf("my size x %g their %g\n", body.size.x(), size_x);

    int fit_pixels = fitting_body.size.y() - padding_split * (v_cnt - 1);
    // printf("total fit pixels: %d for %g weight\n", fit_pixels, overall_weight);

    for (int i = 0; i < v_cnt - 1; ++i) {
        double w = weights[i];
        double coef = w / overall_weight;
        int y_pixels = fit_pixels * coef;
        // printf("y: %d | size: %d\n", cur_min_y, y_pixels);

        if ((size_t) i >= subviews.size()) {
            return;
        }

        AbstractView *view = subviews[i];
        view->get_body().set_position({min_x, static_cast<double>(cur_min_y)});
        view->get_body().set_size({size_x, static_cast<double>(y_pixels)});
        view->recalculate_fit_body();
        view->refit();

        cur_min_y += y_pixels + padding_split;
        fit_pixels -= y_pixels;
        overall_weight -= w;
    }

    // printf("FINAL\n");

    if (v_cnt - 1 >= (int) subviews.size()) {
        return;
    }

    // printf("y: %d | size: %g\n", cur_min_y, fitting_body.size.y() - (double) cur_min_y);

    AbstractView *view = subviews[v_cnt - 1];
    view->get_body().set_position({min_x, static_cast<double>(cur_min_y)});
    view->get_body().set_size({size_x, static_cast<double>(fitting_body.size.y() - (double) cur_min_y)});
    view->recalculate_fit_body();
    view->refit();
}
