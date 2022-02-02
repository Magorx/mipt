#include "labeled_view.h"


AbstractLabledView::AbstractLabledView(const ViewBody &body, AbstractView *parent, bool to_block_covered, bool to_pass_inactive, bool is_shown) :
v_Hideable(body, parent, to_pass_inactive, is_shown, to_block_covered),
v_label(nullptr)
{}

AbstractLabledView::~AbstractLabledView() {
    delete v_label;
}

void AbstractLabledView::add_label(const char *label, int char_size, RGBA font_color, RGBA back_color, bool centrized) {
    v_Text *text = nullptr;
    if (centrized) {
        text = new v_Text(ViewBody{body.size / 2, body.size / 2}, label, char_size, font_color, back_color, centrized);
    } else {
        text = new v_Text(ViewBody{0, 0}, label, char_size, font_color, back_color, centrized);
    }

    if (v_label) {
        delete v_label;
        delete_subview(v_label);
    }

    v_label = text;
    // add_subview(v_label);
}

void AbstractLabledView::render(Renderer *renderer) {
    if (v_label) {
        renderer->shift( body.position);
        v_label->render(renderer);
        renderer->shift(-body.position);
    }
}

void AbstractLabledView::refit() {
    AbstractView::refit();

    if (v_label) {
        v_label->get_body().set_position({body.size / 2});
        v_label->get_body().set_size({body.size / 2});
    }
}

char *AbstractLabledView::get_label_text() {
    if (!v_label) return nullptr;

    return v_label->get_label();
}
