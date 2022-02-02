#pragma once


#include "collections/views/highlighter.h"
#include "redactor/plugin_std/plugin_std.hpp"
#include "redactor/plugins/interface/target.h"


inline ViewBody to_vbody(const PUPPY::WBody &body) {
    return {{body.position.x, body.position.y}, {body.size.x, body.size.y}};
}

inline PUPPY::WBody to_wbody(const ViewBody &body) {
    return {{(float) body.position.x(), (float) body.position.y()}, {(float) body.size.x(), (float) body.size.y()}};
}


#define WIDGET_FIX_(WTYPE)                                \
virtual void set_active(bool flag) override {             \
    if (flag) {                                           \
        widget->on_show({});                              \
    } else {                                              \
        widget->on_hide({});                              \
    }                                                     \
                                                          \
    v_Highlighter::set_active(flag);                      \
}                                                         \
                                                          \
WTYPE *get_widget() { return widget; }                    \
void set_widget(WTYPE *widget_) { widget = widget_; }


class WidgetView : public v_Highlighter {
    PUPPY::Widget *widget;

public:
    WidgetView(const ViewBody &body, PUPPY::Widget *widget);
    virtual ~WidgetView();

    bool to_delete_widget = true;

    WIDGET_FIX_(PUPPY::Widget)
};


template <typename VIEW_T>
class FwdRenderCall : public EventAcceptor<VIEW_T, Event::RenderCall> {
public:
    FwdRenderCall(VIEW_T *widget) : EventAcceptor<VIEW_T, Event::RenderCall>(widget) {}
    virtual EventAccResult operator()(const Event::RenderCall &event, const EventAccResult * = nullptr) override {
        auto acceptor = EventAcceptor<VIEW_T, Event::RenderCall>::acceptor;
        auto widget = acceptor->get_widget();
        widget->on_render({});
        auto texture = dynamic_cast<Target*>(widget->get_texture());

        if (texture) {
            AppearenceTexture appr(texture->get_layer()->get_texture());
            appr.fit_for_size(acceptor->get_body().size);
            event.renderer->set_appearence(&appr);
            event.renderer->apr_draw_rectangle(acceptor->get_body().position, acceptor->get_body().size);

            return EventAccResult::stop;
        }

        return EventAccResult::none;
    }
};


#define FWD_TO_WIDGET_(WTYPE, EVENT_T, METHOD, ARGS)                                                                          \
class WFwd##EVENT_T##WTYPE : public EventAcceptor<WTYPE, Event::EVENT_T> {                                                    \
public:                                                                                                                       \
    WFwd##EVENT_T##WTYPE(WTYPE *widget) : EventAcceptor<WTYPE, Event::EVENT_T>(widget) {}                                     \
                                                                                                                              \
    virtual EventAccResult operator()(const Event::EVENT_T &event, const EventAccResult * = nullptr) override {               \
        auto widget = EventAcceptor<WTYPE, Event::EVENT_T>::acceptor->get_widget(); if (!widget) return EventAccResult::none; \
                                                                                                                              \
        widget->METHOD ARGS;                                                                                                  \
                                                                                                                              \
        return EventAccResult::none;                                                                                          \
    }                                                                                                                         \
};

#define FWD_CLASS_(WTYPE, EVENT_T) WFwd##EVENT_T##WTYPE(this)

#define FWD_ALL_FOR_CLASS_(WTYPE)                             \
FWD_TO_WIDGET_(WTYPE, Tick, on_tick, ({                       \
    event.dt                                                  \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, MousePress, on_mouse_press, ({          \
    {(float) event.position.x(), (float) event.position.y()}, \
    (PUPPY::MouseButton) event.button                         \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, MouseRelease, on_mouse_release, ({      \
    {(float) event.position.x(), (float) event.position.y()}  \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, MouseMove, on_mouse_move, ({            \
    {(float) event.from.x(), (float) event.from.y()},         \
    {(float) event.to.x()  , (float) event.to.y()},           \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, KeyDown, on_key_down, ({                \
    (PUPPY::Keyboard::Key) event.code                         \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, KeyUp, on_key_up, ({                    \
    (PUPPY::Keyboard::Key) event.code                         \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, TextEnter, on_text_enter, ({            \
    (PUPPY::Event::TextEnter) event.keycode                   \
}))                                                           \
                                                              \
FWD_TO_WIDGET_(WTYPE, Scroll, on_scroll, ({                   \
    {(float) event.delta.x(), (float) event.delta.y()},       \
    {(float) event.position.x(), (float) event.position.y()}  \
}))


#define INIT_FWD_TO_WIDGET(WTYPE)                          \
e_render_call.pop_observer();                              \
e_render_call.add(new FwdRenderCall<WTYPE>(this));         \
e_render_call.add(new AVRenderCallAcceptor(this));         \
                                                           \
                                                           \
e_tick.add          (new FWD_CLASS_(WTYPE, Tick));         \
e_mouse_press.add   (new FWD_CLASS_(WTYPE, MousePress));   \
e_mouse_release.add (new FWD_CLASS_(WTYPE, MouseRelease)); \
e_mouse_move.add    (new FWD_CLASS_(WTYPE, MouseMove));    \
e_key_down.add      (new FWD_CLASS_(WTYPE, KeyDown));      \
e_key_up.add        (new FWD_CLASS_(WTYPE, KeyUp));        \
e_text_enter.add    (new FWD_CLASS_(WTYPE, TextEnter));    \
e_scroll.add        (new FWD_CLASS_(WTYPE, Scroll))


class PluginWidget : virtual public PUPPY::Widget {
protected:
    v_Highlighter *view;

public:
    PluginWidget(const ViewBody &body, PUPPY::Widget *parent = nullptr, bool to_spawn_view = false);
    PluginWidget(Widget *widget);
    virtual ~PluginWidget();

    v_Highlighter *get_view() { return view; }
    void set_view(v_Highlighter *view_) { view = view_; }

    virtual bool is_active() override;
    virtual bool is_inside(const PUPPY::Vec2f &pos) override;

    virtual bool add_child(Widget *child) override;
    virtual bool delete_child(Widget *child) override;

    virtual void set_to_delete() override { view->set_to_delete(true); }

    bool add_child(PluginWidget *child);
    bool delete_child(PluginWidget *child);
    virtual bool delete_from_parent() override;
    
    virtual void on_render          (const PUPPY::Event::Render          &) override {}
    virtual void on_tick            (const PUPPY::Event::Tick            &) override {}
    virtual void on_mouse_press     (const PUPPY::Event::MousePress      &) override {}
    virtual void on_mouse_release   (const PUPPY::Event::MouseRelease    &) override {}
    virtual void on_mouse_move      (const PUPPY::Event::MouseMove       &) override {}
    virtual void on_key_down        (const PUPPY::Event::KeyDown         &) override {}
    virtual void on_key_up          (const PUPPY::Event::KeyUp           &) override {}
    virtual void on_text_enter      (const PUPPY::Event::TextEnter       &) override {}
    virtual void on_scroll          (const PUPPY::Event::Scroll          &) override {}
    virtual void on_hide            (const PUPPY::Event::Hide            &) override {}
    virtual void on_show            (const PUPPY::Event::Show            &) override {}

    virtual void hide()  override;
    virtual void show()  override;
    virtual void focus() override;

    virtual void set_caption(const char *text, size_t font_size, const PUPPY::Vec2f *pos = nullptr) override;
    virtual void set_base_color(const PUPPY::RGBA &color) override;

    virtual void *get_extra_data(void *) override { return nullptr; }

// ----------------

    virtual void set_position(const PUPPY::Vec2f &position_) override { body.position = position_; view->get_body() = to_vbody(body); }
    virtual void set_size(const PUPPY::Vec2f &size_) override { body.size = size_; view->get_body() = to_vbody(body); }

    virtual PUPPY::WBody get_body() override { return body; }
    virtual void set_body(const PUPPY::WBody &body_) override { body = body_; view->get_body() = to_vbody(body); }

    virtual Widget *get_parent() const override { return parent; }
    virtual void set_parent(Widget *parent_) override { parent = parent_; }

    virtual PUPPY::RenderTarget *get_texture() override { return texture; }
    virtual void set_texture(PUPPY::RenderTarget *texture_) override { texture = texture_; }

protected:
    PUPPY::WBody body;
    Widget *parent;
    PUPPY::RenderTarget *texture;
};
