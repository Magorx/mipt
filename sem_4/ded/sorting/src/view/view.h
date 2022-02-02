#pragma once


#include "engine/tickable.h"
#include "visual/renderer/renderable_object.h"
#include "visual/renderer/appearence.h"

#include "utils/vec3d.h"
#include "utils/vec2d.h"

#include "lambda.h"

#include "event_system/event_system.h"

#include "visual/style/mouse_reaction_style.h"

#include <vector>


class AbstractView;


class AVMissPressBlocker : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVMissPressBlocker(AbstractView *button);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVMissReleaseBlocker : public EventAcceptor<AbstractView, Event::MouseRelease> {
public:
    AVMissReleaseBlocker(AbstractView *button);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class AVMissMoveBlocker : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVMissMoveBlocker(AbstractView *button);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class AVMissScrollBlocker : public EventAcceptor<AbstractView, Event::Scroll> {
public:
    AVMissScrollBlocker(AbstractView *button);

    EventAccResult operator()(const Event::Scroll &event, const EventAccResult *cur_res = nullptr) override;
};


class AVRenderCallAcceptor : public EventAcceptor<AbstractView, Event::RenderCall> {
public:
    AVRenderCallAcceptor(AbstractView *view);

    EventAccResult operator()(const Event::RenderCall &event, const EventAccResult *cur_res = nullptr) override;
};

class AVDragAcceptor : public EventAcceptor<AbstractView, Event::MouseDrag> {
    bool allow_out_of_bounds;
public:
    AVDragAcceptor(AbstractView *view, bool allow_out_of_bounds = true);

    EventAccResult operator()(const Event::MouseDrag &event, const EventAccResult *cur_res = nullptr) override;
};

class AVDragEmitter : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVDragEmitter(AbstractView *view);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class AVPressFocuser : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVPressFocuser(AbstractView *view);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredMoveBlocker : public EventAcceptor<AbstractView, Event::MouseMove> {
public:
    AVCoveredMoveBlocker(AbstractView *view);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredPressBlocker : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVCoveredPressBlocker(AbstractView *view);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredReleaseBlocker : public EventAcceptor<AbstractView, Event::MouseRelease> {
public:
    AVCoveredReleaseBlocker(AbstractView *view);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCoveredScrollBlocker : public EventAcceptor<AbstractView, Event::Scroll> {
public:
    AVCoveredScrollBlocker(AbstractView *view);

    EventAccResult operator()(const Event::Scroll &event, const EventAccResult *cur_res = nullptr) override;
};

class AVCloseAcceptor : public EventAcceptor<AbstractView, Event::Close> {
public:
    AVCloseAcceptor(AbstractView *view);

    EventAccResult operator()(const Event::Close &event, const EventAccResult *cur_res = nullptr) override;
};

class AVAnimatorPress : public EventAcceptor<AbstractView, Event::MousePress> {
    MouseReactionStyle *style;
public:
    AVAnimatorPress(AbstractView *view, MouseReactionStyle *style);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};

class AVAnimatorMove : public EventAcceptor<AbstractView, Event::MouseMove> {
    MouseReactionStyle *style;
public:
    AVAnimatorMove(AbstractView *view, MouseReactionStyle *style);

    EventAccResult operator()(const Event::MouseMove &event, const EventAccResult *cur_res = nullptr) override;

    inline MouseReactionStyle *get_style() { return style; }
};

class AVAnimatorRelease : public EventAcceptor<AbstractView, Event::MouseRelease> {
    MouseReactionStyle *style;
public:
    AVAnimatorRelease(AbstractView *view, MouseReactionStyle *style);

    EventAccResult operator()(const Event::MouseRelease &event, const EventAccResult *cur_res = nullptr) override;

    inline MouseReactionStyle *get_style() { return style; }
};

class AVSelectableFocuser : public EventAcceptor<AbstractView, Event::KeyDown> {
public:
    AVSelectableFocuser(AbstractView *view);

    EventAccResult operator()(const Event::KeyDown &event, const EventAccResult *cur_res = nullptr) override;
};

class AVSelectablePressDefocuser : public EventAcceptor<AbstractView, Event::MousePress> {
public:
    AVSelectablePressDefocuser(AbstractView *view);

    EventAccResult operator()(const Event::MousePress &event, const EventAccResult *cur_res = nullptr) override;
};


struct ViewBody {
    Vec2d position;
    Vec2d size;

    inline Vec2d get_position() const { return position; }
    inline Vec2d get_size() const { return size; }

    inline void set_position(Vec2d position_) { position = position_; }
    inline void set_size(Vec2d size_) { size = size_; }

    inline bool is_inside(Vec2d click) { return click.x() >= 0 && click.y() >= 0 && click.x() < size.x() && click.y() < size.y(); }
};


class AbstractView : public RenderableObject, public EventSystem {
protected:
    const char *view_id;

    ViewBody body;
    ViewBody fit_body;
    
    AbstractView *parent;
    std::vector<AbstractView*> subviews;

    Appearence *appearence;

    friend AVMissPressBlocker;
    friend AVMissMoveBlocker;
    friend AVMissReleaseBlocker;
    friend AVCoveredPressBlocker;
    friend AVCoveredMoveBlocker;
    friend AVCoveredReleaseBlocker;
    friend AVCoveredScrollBlocker;
    friend AVDragAcceptor;
    friend AVDragEmitter;
    friend AVSelectableFocuser;

    bool pressed;
    bool focuseable;
    
    bool selectable;
    bool selectable_blocking_node;
    bool selected;
    bool to_draw_selected_bounds;
    bool tab_selected;
    
    bool appearenced;
    bool cursor_inside;
    bool covering_block;

    bool to_subrender;

    AbstractView *next_by_idx(int idx, int direction);
    AbstractView *get_selectable_blocking_node();
    AbstractView *traverse_for_selectable(int idx, int direction, bool from_parent = false);

    int get_idx(AbstractView *child);

public:
    AbstractView(ViewBody body, AbstractView *parent = nullptr, bool to_block_covered = false);
    virtual ~AbstractView();

    virtual void add_subview(AbstractView *subview);
    void delete_subview(AbstractView *view);
    void delete_subview(size_t index);

    virtual void set_to_delete(bool to_delete_, bool to_supress_delete_event = false) override;

    virtual void render(Renderer *renderer) override;
    virtual void subrender(Renderer *renderer);

    virtual bool is_active() const { return true; };

    void fit(const Vec2d &left_up, const Vec2d &right_down, bool absolute_fit = false);
    void fit_proportional(const Vec2d &left_up, const Vec2d &right_down);
    void fit_absolute(const Vec2d &left_up, const Vec2d &right_down);

    virtual void recalculate_fit_body();
    virtual void refit();
    void refit_in_parent();

    virtual bool is_inside(const Vec2d &click) { return body.is_inside(click); }
    virtual bool is_inside(const Vec2d &from, const Vec2d &to) { return body.is_inside(from) || body.is_inside(to); }

    inline void set_parent(AbstractView *parent_) { parent = parent_; }

    inline bool is_pressed() const { return pressed; }
    inline bool is_cursor_inside() const { return cursor_inside; }

    inline bool is_focuseable() { return focuseable; }
    inline void set_focuseable(bool focuseable_) { focuseable = focuseable_; }

    inline bool is_selectable() { return selectable; }
    inline void set_selectable(bool selectable_) { selectable = selectable_; }

    inline bool is_selected() { return selected; }
    inline void set_selected(bool selected_) { selected = selected_; }

    virtual void select(bool tabbed = false) { focus(); selected = true; tab_selected = tabbed; }
    virtual void deselect() { selected = false; }

    inline void set_appearence(Appearence *appearence_, bool activate = true) { appearenced = true; appearence = appearence_; if (appearence && activate) appearence->activate(); }

    inline void set_view_id(const char *view_id_) { view_id = view_id_; }
    inline const char *get_view_id() { return view_id; }

    inline void set_to_subrender(bool to_subrender_) { to_subrender = to_subrender_; }

    inline Appearence *get_appearence() { return appearence; }

    inline MogaEngine *get_engine() { return App.engine; }

    inline void centrize(ViewBody &other_body) {
        other_body.position = body.size / 2 - other_body.size / 2;
    }

    std::vector<AbstractView*> &get_subviews() {
        return subviews;
    }

    ViewBody &get_body();

    AbstractView *get_next_selectable(bool from_parent = false);
    AbstractView *get_prev_selectable(bool from_parent = false);

    virtual AbstractView *get_first_selectable(bool from_parent = false);
    virtual AbstractView *get_last_selectable(bool from_parent = false);

};


template <typename EVENT_T>
class AVCloseGenerator : public EventAcceptor<AbstractView, EVENT_T> {
    using EventAcceptor<AbstractView, EVENT_T>::acceptor;

public:
    AVCloseGenerator(AbstractView *acceptor) : EventAcceptor<AbstractView, EVENT_T>(acceptor) {}

    EventAccResult operator()(const EVENT_T &, const EventAccResult *) {
        acceptor->e_close.emit({});
        return EventAccResult::cont;
    }
};


template <typename EVENT_T>
class AVToggleActivityGenerator : public EventAcceptor<AbstractView, EVENT_T> {
    using EventAcceptor<AbstractView, EVENT_T>::acceptor;

public:
    AVToggleActivityGenerator(AbstractView *acceptor) : EventAcceptor<AbstractView, EVENT_T>(acceptor) {}

    EventAccResult operator()(const EVENT_T &, const EventAccResult *) {
        acceptor->e_toggle_activity.emit({Event::ActivityToggle::toggle});
        return EventAccResult::cont;
    }
};
