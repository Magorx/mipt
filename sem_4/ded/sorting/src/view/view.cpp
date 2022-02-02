#include "view.h"
#include <memory>
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>


AbstractView::AbstractView(ViewBody body, AbstractView *parent, bool to_block_covered):
view_id(nullptr),
body(body),
fit_body(body),
parent(parent),
appearence(nullptr),
pressed(false),
focuseable(false),
selectable(false),
selectable_blocking_node(false),
selected(false),
to_draw_selected_bounds(true),
tab_selected(false),
appearenced(false),
cursor_inside(false),
covering_block(false),
to_subrender(true)
{
    e_render_call.add(new AVRenderCallAcceptor(this));
    
    e_mouse_press.add(new AVPressFocuser(this));

    if (to_block_covered) {
        e_mouse_press.add(new AVCoveredPressBlocker(this), false);
        e_mouse_release.add(new AVCoveredReleaseBlocker(this), false);
        e_mouse_move.add(new AVCoveredMoveBlocker(this), false);
        e_scroll.add(new AVCoveredScrollBlocker(this));
    }

    e_mouse_press.set_event_affector([this](const Event::MousePress &event)     { return Event::MousePress   {event.position - this->body.position, event.button}; } );
    e_mouse_release.set_event_affector([this](const Event::MouseRelease &event) { return Event::MouseRelease {event.position - this->body.position, event.button}; } );
    e_mouse_move.set_event_affector([this](const Event::MouseMove &event)       { return Event::MouseMove    {event.from - this->body.position, event.to - this->body.position}; } );
    e_scroll.set_event_affector([this](const Event::Scroll &event)              { return Event::Scroll {event.delta, event.position - this->body.position}; } );


    if (parent) {
        parent->add_subview(this);

        if (body.position.len_squared() < 2) {
            if (body.size.len_squared() < 2) {
                fit(fit_body.position, fit_body.size);
            } else {
                body.position = parent->get_body().get_size() * body.position;
            }
        } else {
            Vec2d p_size = parent->get_body().size;
            fit_body.position = body.position / p_size;
            fit_body.size = (body.position + body.size) / p_size;
        }
    }
}

AbstractView::~AbstractView() {
    for (auto &sub : subviews) {
        sub->set_parent(nullptr);
    }

    if (parent) {
        parent->delete_subview(this); // FIXME idk if its a bug or not, nvm
    }
}

void AbstractView::set_to_delete(bool to_delete_, bool to_supress_delete_event) {
    to_delete = to_delete_;
    if (to_delete_ && !to_supress_delete_event) {
        e_close.emit({}, false, true);
    }
}

void AbstractView::render(Renderer *renderer) {
    subrender(renderer);
}

void AbstractView::subrender(Renderer *renderer) {
    if (!to_subrender) return;

    renderer->shift(body.position);

    e_render_call.dispatch_to_sub_es({renderer}, true);

    renderer->shift(-body.position);
}

void AbstractView::add_subview(AbstractView *subview) {
    if (!subview) return;

    subviews.push_back(subview);
    add_es(subview);
    subview->set_parent(this);

    Vec2d p_size = body.size;
    subview->fit_body.position = subview->get_body().position / p_size;
    subview->fit_body.size = (subview->get_body().position + subview->get_body().size) / p_size;

    const bool to_focus = true;
    if (to_focus) subview->focus();
}

void AbstractView::delete_subview(AbstractView *view) {
    size_t i = 0;
    size_t views_cnt = subviews.size();

    for (; i < views_cnt && subviews[i] != view; ++i);

    if (i < views_cnt) {
        // printf("CHECK delete_subview IF FAILS\n");
        // delete subviews[i];
        std::swap(subviews[i], subviews[views_cnt - 1]);
        subviews.pop_back();
    }
}

void AbstractView::delete_subview(size_t index) {
    if (index < subviews.size()) {
        std::swap(subviews[index], subviews[subviews.size()]);
        subviews.pop_back();
    }
}

ViewBody &AbstractView::get_body() {
    return body;
}

void AbstractView::fit(const Vec2d &left_up, const Vec2d &right_down, bool absolute_fit) {
    if (absolute_fit) {
        fit_absolute(left_up, right_down);
    } else {
        fit_proportional(left_up, right_down);
    }
    
    recalculate_fit_body();
    refit();
}

void AbstractView::fit_proportional(const Vec2d &left_up, const Vec2d &right_down) {
    if (!parent) {
        return;
    }
    
    ViewBody &p_body = parent->get_body();

    body.position = p_body.get_size() * left_up;
    body.size     = p_body.get_size() * (right_down - left_up);
}

void AbstractView::fit_absolute(const Vec2d &left_up, const Vec2d &right_down) {
    if (!parent) {
        return;
    }

    body.position = left_up;
    body.size     = right_down - body.position;

    recalculate_fit_body();
}

void AbstractView::refit() {
    refit_in_parent();
    for (auto subview : subviews) {
        subview->refit();
    }
}

void AbstractView::refit_in_parent() {
    fit_proportional(fit_body.position, fit_body.size);
}

void AbstractView::recalculate_fit_body() {
    if (!parent) return;

    Vec2d p_size = parent->get_body().size;
    fit_body.position = body.position / p_size;
    fit_body.size = (body.position + body.size) / p_size;
}

int AbstractView::get_idx(AbstractView *child) {
    for (int i = 0; i < (int) subviews.size(); ++i) {
        if (subviews[i] == child) {
            return i;
        }
    }

    return -1;
}

AbstractView *AbstractView::next_by_idx(int idx, int direction) { 
    int next_idx = idx + direction;
    if (next_idx < 0 || next_idx >= (int) subviews.size()) {
        return nullptr;
    } else {
        return subviews[next_idx];
    }
}

AbstractView *AbstractView::traverse_for_selectable(int idx, int direction, bool from_parent) {
    if (is_selectable()) return this;

    AbstractView *next = nullptr;
    while ((next = next_by_idx(idx, direction))) {
        idx += direction;

        if (!next->is_active()) {
            continue;
        }

        if (next->is_selectable()) {
            return next;
        }
        if (next->selectable_blocking_node) {
            continue;
        }

        AbstractView *ret = nullptr;
        if (direction < 0) {
            ret = next->get_prev_selectable(true);
        } else {
            ret = next->get_next_selectable(true);
        }

        if (ret) {
            return ret;
        }
    }

    if (!selectable_blocking_node && !from_parent && parent) {
        int my_idx = parent->get_idx(this);
        if (my_idx < 0) return nullptr;

        return parent->traverse_for_selectable(my_idx, direction);
    }

    return nullptr;
}

AbstractView *AbstractView::get_next_selectable(bool from_parent) {
    for (int i = 0; i < (int) subviews.size(); ++i) {
        AbstractView *ret = subviews[i]->traverse_for_selectable(-1, +1, true);
        if (ret) return ret;
    }

    if (!from_parent && parent) {
        int my_idx = parent->get_idx(this);
        if (my_idx < 0) return nullptr;

        return parent->traverse_for_selectable(my_idx, +1); 
    }

    return nullptr;
}

AbstractView *AbstractView::get_prev_selectable(bool from_parent) {
    for (int i = (int) subviews.size() - 1; i >= 0; --i) {
        AbstractView *ret = subviews[i]->traverse_for_selectable((int) subviews[i]->subviews.size(), -1, true);
        if (ret) return ret;
    }

    if (!from_parent && parent) {
        int my_idx = parent->get_idx(this);
        if (my_idx < 0) return nullptr;

        return parent->traverse_for_selectable(my_idx, -1); 
    }

    return nullptr;
}

AbstractView *AbstractView::get_selectable_blocking_node() {
    if (selectable_blocking_node || !parent) return this;

    return parent->get_selectable_blocking_node();
}

AbstractView *AbstractView::get_first_selectable(bool from_parent) {
    AbstractView *blocking_node = from_parent ? this : get_selectable_blocking_node();
    return blocking_node->traverse_for_selectable(-1, +1, true);
}

AbstractView *AbstractView::get_last_selectable(bool from_parent) {
    AbstractView *blocking_node = from_parent ? this : get_selectable_blocking_node();
    return blocking_node->traverse_for_selectable(blocking_node->subviews.size(), -1);
}

AVMissPressBlocker::AVMissPressBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissPressBlocker::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.position)) {
        return EventAccResult::stop;
    } else {
        acceptor->pressed = true;
    }

    return EventAccResult::cont;
}

AVMissScrollBlocker::AVMissScrollBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissScrollBlocker::operator()(const Event::Scroll &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.position)) {
        return EventAccResult::stop;
    }

    return EventAccResult::none;
}

AVMissReleaseBlocker::AVMissReleaseBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissReleaseBlocker::operator()(const Event::MouseRelease &event, const EventAccResult *) {
    acceptor->pressed = false;
 
    if (!acceptor->is_inside(event.position)) {
        return EventAccResult::stop;
    }

    return EventAccResult::none;
}


AVMissMoveBlocker::AVMissMoveBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVMissMoveBlocker::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (!acceptor->is_inside(event.from) && !acceptor->is_inside(event.to)) {
        return EventAccResult::stop;
    }
    
    return EventAccResult::cont;
}

AVRenderCallAcceptor::AVRenderCallAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVRenderCallAcceptor::operator()(const Event::RenderCall &event, const EventAccResult *) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    acceptor->render(event.renderer);

    return EventAccResult::stop;
}

AVDragAcceptor::AVDragAcceptor(AbstractView *av, bool allow_out_of_bounds) : EventAcceptor(av), allow_out_of_bounds(allow_out_of_bounds) {}

EventAccResult AVDragAcceptor::operator()(const Event::MouseDrag &event, const EventAccResult *) {
    acceptor->get_body().position += event.to - event.from;

    if (!allow_out_of_bounds) {
        acceptor->get_body().position.content[1] = fmax(0, acceptor->get_body().position.content[1]);
    }

    return EventAccResult::stop;
}

AVDragEmitter::AVDragEmitter(AbstractView *av) : EventAcceptor(av) {
    av->e_mouse_press.add(new AVMissPressBlocker(av));
    av->e_mouse_release.add(new AVMissReleaseBlocker(av));
}

EventAccResult AVDragEmitter::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (acceptor->pressed) {
        acceptor->e_mouse_drag.emit({event.from, event.to, Event::MouseDrag::left});
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

AVPressFocuser::AVPressFocuser(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVPressFocuser::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->is_active()) return EventAccResult::none;

    if (acceptor->is_inside(event.position) && acceptor->is_focuseable()) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::focus); // | EventAccResult::prevent_siblings_dispatch);
    } else {
        return EventAccResult::none;
    }
}

AVCloseAcceptor::AVCloseAcceptor(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCloseAcceptor::operator()(const Event::Close &, const EventAccResult *) {  
    acceptor->set_to_delete(true);
    return EventAccResult::none;
}

AVCoveredPressBlocker::AVCoveredPressBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredPressBlocker::operator()(const Event::MousePress &event, const EventAccResult *prev_res) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    if (acceptor->is_inside(event.position) && ((prev_res && (*prev_res & EventAccResult::cont)) || acceptor->covering_block)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

AVCoveredReleaseBlocker::AVCoveredReleaseBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredReleaseBlocker::operator()(const Event::MouseRelease &event, const EventAccResult *prev_res) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    if (acceptor->is_inside(event.position) && ((prev_res && (*prev_res & EventAccResult::cont)) || acceptor->covering_block)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

AVCoveredMoveBlocker::AVCoveredMoveBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredMoveBlocker::operator()(const Event::MouseMove &event, const EventAccResult *prev_res) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    if (acceptor->is_inside(event.to)) {
        acceptor->cursor_inside = true;
    } else {
        acceptor->cursor_inside = false;
    }

    if (acceptor->is_inside(event.from) && acceptor->is_inside(event.to) && ((prev_res && (*prev_res & EventAccResult::cont)) || acceptor->covering_block)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }
    
    return EventAccResult::none;
}

AVCoveredScrollBlocker::AVCoveredScrollBlocker(AbstractView *av) : EventAcceptor(av) {}

EventAccResult AVCoveredScrollBlocker::operator()(const Event::Scroll &event, const EventAccResult *prev_res) {
    if (!acceptor->is_active()) return EventAccResult::stop;

    if (acceptor->is_inside(event.position) && ((prev_res && (*prev_res & EventAccResult::cont)) || acceptor->covering_block)) {
        return (EventAccResult) (EventAccResult::cont | EventAccResult::prevent_siblings_dispatch);
    }

    return EventAccResult::none;
}

AVAnimatorPress::AVAnimatorPress(AbstractView *view, MouseReactionStyle *style) : EventAcceptor(view), style(style) {}

EventAccResult AVAnimatorPress::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (acceptor->is_inside(event.position) && !acceptor->is_pressed()) {
        acceptor->set_appearence(style->press);
    }

    return EventAccResult::none;
}

AVAnimatorMove::AVAnimatorMove(AbstractView *view, MouseReactionStyle *style) :
EventAcceptor(view), style(style) {}

EventAccResult AVAnimatorMove::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (acceptor->is_cursor_inside()) {
        if (!acceptor->is_inside(event.from)) {
            acceptor->set_appearence(style->hover);
        }
    } else {
        if (acceptor->is_inside(event.from)) {
            acceptor->set_appearence(style->unhover);
        }
    }

    return EventAccResult::none;
}

AVAnimatorRelease::AVAnimatorRelease(AbstractView *view, MouseReactionStyle *style) :
EventAcceptor(view), style(style) {}

EventAccResult AVAnimatorRelease::operator()(const Event::MouseRelease &, const EventAccResult *) {
    if (acceptor->is_pressed()) {
        acceptor->set_appearence(style->release);
    }

    return EventAccResult::none;
}

AVSelectableFocuser::AVSelectableFocuser(AbstractView *view) :
EventAcceptor(view)
{}

EventAccResult AVSelectableFocuser::operator()(const Event::KeyDown &event, const EventAccResult *) {
    if (!(event.code == Keyboard::Key::tab) || !(acceptor->is_selected() || acceptor->selectable_blocking_node)) {
        return EventAccResult::none;
    }
    AbstractView *view = acceptor;

    if (!Keyboard::is_pressed_shift()) {
        AbstractView *next = acceptor->selectable_blocking_node ? acceptor->get_first_selectable() : view->get_next_selectable();
        if (!next) {
            next = view->get_first_selectable();
        }

        if (next) {
            if (view->is_selected()) view->deselect();
            next->select(true);
        }
    } else {
        AbstractView *prev = acceptor->selectable_blocking_node ? acceptor->get_last_selectable() : view->get_prev_selectable();
        if (!prev) {
            prev = view->get_last_selectable();
        }

        if (prev) {
            if (view->is_selected()) view->deselect();
            prev->select(true);
        }
    }

    return EventAccResult::done;
}


AVSelectablePressDefocuser::AVSelectablePressDefocuser(AbstractView *view) : EventAcceptor(view) {}

EventAccResult AVSelectablePressDefocuser::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->is_selected()) return EventAccResult::none;

    if (!acceptor->is_inside(event.position)) acceptor->deselect();

    return EventAccResult::cont;
}
