#include "window.h"


v_Window::v_Window(const char *name, const ViewBody &body, WindowStyle *style, bool draggable, double header_size, AbstractView *parent) :
v_Highlighter({body.position, {body.size.x(), body.size.y() + header_size}}, nullptr, parent, 0, true),
header(new v_UtilityTab({body.size.x() + PX_WINDOW_PADDING * 2, header_size}, style ? style->header : nullptr)),
content(new v_Highlighter({{0, 0}, body.size}, nullptr, nullptr, 0)),
frame(nullptr),
text_color(RColor{255, 255, 255})
{
    if (style->body) {
        frame = new v_Highlighter({-padding, body.size + Vec2d{padding * 2, 2 * padding}}, nullptr, nullptr, 0);
        frame->set_appearence(style->body);
        add_subview(frame);
    }

    header->get_body().position.content[1] -= header_size + PX_UTIL_BUTTON_SIZE;
    header->get_body().position.content[0] -= padding;
    add_subview(header);
    
    if (draggable) header->get_drag_pod()->e_mouse_drag.add(new AVDragAcceptor(this, false));

    header->get_button_close()->e_close.add(new AVCloseAcceptor(this));
    header->get_button_hide()->e_toggle_activity.add(new HideableActivityToggleAcceptor(this));
    
    header->add_label(name, App.font.size.basic_header, text_color.rgb());
    
    set_focuseable(true);
    selectable_blocking_node = true;
    covering_block = true;
    e_key_down.add(new AVSelectableFocuser(this), false);

    auto content_appr = new AppearenceColor({230, 230, 230, 255});
    App.add_appr(content_appr);
    content->set_appearence(content_appr);
    add_subview(content);

    header->focus();
    content->focus();
    header->set_focuseable(true);
    content->set_focuseable(false);
}

void v_Window::toggle_hide_button() {
    get_header()->get_button_hide()->toggle();
}

void v_Window::toggle_close_button() {
    get_header()->get_button_close()->toggle();
}

void v_Window::add_subview(AbstractView *subview) {
    if (!subview) return;

    subview->get_body().position.content[1] += header->get_body().size.content[1];
    subview->get_body().position.content[0] += padding;
    v_Highlighter::add_subview(subview);
}

void v_Window::set_content_color(RColor color) {
    auto content_appr = new AppearenceColor(color);
    App.add_appr(content_appr);
    content->set_appearence(content_appr);
}

void v_Window::fit_frame_to_content() {
    if (!frame || !content) return;

    frame->get_body().size = content->get_body().size + 2 * PX_WINDOW_PADDING;
}

AbstractView *v_Window::get_first_selectable(bool) {
    auto first = content->get_first_selectable(true);
    if (first) return first;
    
    first = header->get_first_selectable();
    return first;
}

AbstractView *v_Window::get_last_selectable(bool) {
    auto last = content->get_last_selectable(true);
    if (last) return last;
    
    last = header->get_last_selectable();
    return last;
}

bool v_Window::is_inside(const Vec2d &click) {
    return header->is_inside(click - header->get_body().position) || content->is_inside(click - content->get_body().position);
}

void v_Window::set_name(const char *name) {
    header->add_label(name, App.font.size.basic_header, text_color.rgb());
}

const char *v_Window::get_name() {
    return header->get_label_text();
}
