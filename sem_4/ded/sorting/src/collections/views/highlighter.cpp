#include "highlighter.h"


const double HIGHLIGHTER_ON_COEF = 0.0;


v_Highlighter::v_Highlighter(const ViewBody &body, SmartColor *color, AbstractView *parent, double highlight_coef, bool to_block_covered):
AbstractLabledView(body, parent, to_block_covered),
highlight_coef(highlight_coef),
color(color)
{
    e_mouse_press.add(new HighlighterPressAcceptor(this));
    e_mouse_move.add(new HighlighterMoveAcceptor(this));
    e_toggle_activity.add(new HighlighterDeactivateVisualy(this));
}

v_Highlighter::v_Highlighter(const ViewBody &body, AbstractView *parent, bool to_block_covered):
AbstractLabledView(body, parent, to_block_covered),
highlight_coef(0),
color(nullptr)
{
    e_mouse_press.add(new HighlighterPressAcceptor(this));
    e_mouse_move.add(new HighlighterMoveAcceptor(this));
    e_toggle_activity.add(new HighlighterDeactivateVisualy(this));
}

v_Highlighter::~v_Highlighter() {}

void v_Highlighter::render(Renderer *renderer) {
    if (appearence) {
        appearence->fit_for_size(body.size);
        renderer->set_appearence(appearence);
        renderer->apr_draw_rectangle(body.position, body.size);
    } else if (color) {
        RGBA cur_color = color->rgb();

        renderer->draw_rectangle(body.position, body.size, cur_color);
    }

    subrender(renderer);
    AbstractLabledView::render(renderer);

    if (to_draw_selected_bounds && is_selected() && tab_selected) {
        renderer->draw_rectangle(body.position, body.size, {0, 0, 0, 0}, {255, 0, 0, 255});
    }
}

HighlighterPressAcceptor::HighlighterPressAcceptor(v_Highlighter *highlighter) : EventAcceptor(highlighter) {}

EventAccResult HighlighterPressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    v_Highlighter *hl = acceptor;

    if (hl->is_inside(event.position)) {
        hl->cursor_inside = true;
        return EventAccResult::cont;
    } else {
        hl->cursor_inside = false;
        return EventAccResult::none;
    }
}

HighlighterMoveAcceptor::HighlighterMoveAcceptor(v_Highlighter *highlighter) : EventAcceptor(highlighter) {}

EventAccResult HighlighterMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    v_Highlighter *hl = acceptor;

    if (hl->is_inside(event.to, event.from)) {
        hl->cursor_inside = hl->is_inside(event.to);
        return EventAccResult::cont;
    } else {
        return EventAccResult::none;
    }
}

HighlighterDeactivateVisualy::HighlighterDeactivateVisualy(v_Highlighter *highlighter) : EventAcceptor(highlighter) {}

EventAccResult HighlighterDeactivateVisualy::operator()(const Event::ActivityToggle &event, const EventAccResult *) {
    v_Highlighter *hl = acceptor;

    if ((event.mode & Event::ActivityToggle::State::visualy) && (event.mode & Event::ActivityToggle::State::off)) {
        hl->cursor_inside = false;
    }

    return EventAccResult::none;
}
