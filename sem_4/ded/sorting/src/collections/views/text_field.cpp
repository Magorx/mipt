#include "text_field.h"


v_TextField::v_TextField(const ViewBody &body, TextStyle *style,
                         RColor frame_color, RColor content_color,
                         bool autofit_to_text_size, bool redactable, bool autofit_text_size) :
v_Highlighter({body.position, body.size}),
line(10),
style(style),
v_content(new v_Highlighter({PX_TF_PADDING, this->body.size - PX_TF_PADDING * 2})),
v_cursor(new v_Highlighter({0, {2, body.size.y()}})),
v_selection(new v_Highlighter({0, 0})),
redactable(redactable)
{
    if (autofit_to_text_size) {
        this->body.size = Vec2d{body.size.x(), style->size + PX_TF_PADDING * 4.0};
        v_content->get_body().size = Vec2d{body.size.x() - PX_TF_PADDING * 2.0, this->body.size.y() - PX_TF_PADDING * 2.0};
    } else if (autofit_text_size) {
        style->size = body.size.y() - PX_TF_PADDING * 2.0;
    }

    set_focuseable(redactable);
    set_selectable(redactable);

    to_draw_selected_bounds = false;
    e_key_down.add(new AVSelectableFocuser(this));
    // e_mouse_press.add(new AVSelectablePressDefocuser(this));

    set_appearence(App.add_appr(new AppearenceColor(frame_color)));

    e_key_down.add(new KeyDownTextFieldAcceptor(this));
    e_key_up.add(new KeyUpTextFieldAcceptor(this));
    e_text_enter.add(new TextEnterAcceptor(this));
    e_mouse_press.add(new TextFieldMousePressAcceptor(this));
    e_mouse_release.add(new TextFieldMouseReleaseAcceptor(this));
    e_mouse_move.add(new TextFieldMouseMoveAcceptor(this));

    display();

    v_cursor->set_appearence(
        App.create_animation(
            {App.create_color(App.color.text_field.basic.cursor), 
             App.create_color({0, 0, 0, 0})},
             SC_TF_BLINK, true
        )
    );

    v_selection->set_appearence(App.add_appr(new AppearenceColor(App.color.text_field.basic.selection)));

    add_subview(v_content);
    v_content->set_appearence(App.add_appr(new AppearenceColor(content_color)));
}

v_TextField::~v_TextField() {
    // delete style;
}

void v_TextField::render(Renderer *renderer) {
    v_Text *my_label = v_label;
    v_label = nullptr;
    v_Highlighter::render(renderer);
    v_label = my_label;

    Vec2d shift = body.position + v_label->get_body().position + v_content->get_body().position + Vec2d{PX_TF_PADDING, 0};
    renderer->shift(shift);

    if (selected) {
        v_selection->render(renderer);
    }

    v_label->render(renderer);

    if (selected) {
        v_cursor->render(renderer);
    }

    renderer->shift(-shift);
}

void v_TextField::refit() {
    v_Highlighter::refit();

    v_content->get_body().position = PX_TF_PADDING;
    v_content->get_body().size     = body.size - PX_TF_PADDING * 2;

    display();
}

void v_TextField::select(bool tabbed) {
    AbstractView::select(tabbed);

    if (tabbed) {
        line.select_all(false);
    }

    display();
}

void v_TextField::deselect() {
    AbstractView::deselect();
    e_text_changed.emit({line.c_str(), false});
}

void v_TextField::deselect_text_field(bool input_complete) {
    AbstractView::deselect();

    e_text_changed.emit({line.c_str(), input_complete});
}

Vec2d v_TextField::char_pos(int idx) {
    return Renderer::get_char_position(line.c_str(), idx, style->size, style->font);
}

void v_TextField::display() {
    add_label(line.c_str(), style->size, style->foreground, style->background, false);

    Vec2d pos_cursor = char_pos(line.cursor_pos());
    Vec2d pos_anchor = char_pos(line.anchor_pos());

    pos_cursor += Vec2d{0, style->size / 5.0}; // + v_content->get_body().position;
    pos_anchor += Vec2d{0, style->size / 5.0}; // + v_content->get_body().position;

    v_cursor->get_body().position.content[0] = pos_cursor.x();
    v_cursor->get_body().size.content[1] = body.size.y() - PX_TF_PADDING * 2;

    v_selection->get_body().position = pos_cursor;
    v_selection->get_body().size = pos_anchor - pos_cursor;
    v_selection->get_body().size.content[1] = style->size;

    if (v_cursor->get_appearence()) v_cursor->get_appearence()->activate();
}

void v_TextField::put_cursor_under_mouse(const Vec2d &mouse_pos) {
    int l = 0;
    int r = line.len();
    while (l + 1 < r) {
        int m = (l + r) / 2;
        if (char_pos(m).x() > mouse_pos.x()) {
            r = m;
        } else {
            l = m;
        }
    }
    line.cursor_set(l);
    display();
}

void v_TextField::add_char(char c) {
    if (!redactable) return;

    if (insert) {
        line.put_char(c);
    } else {
        line.insert_char(c);
    }

    line.free_anchors();
    if (!line.cursor_mag()) {
        line.cursor_r();
    }

    if (selection_online) {
        line.fix_anchors();
    }
}

void v_TextField::set_string(const char *str, bool to_fit_width) {
    line.set_str(str);
    display();

    if (to_fit_width) {
        fit_width_to_string();
    }

    e_text_changed.emit({line.c_str()});
}

void v_TextField::set_number(const double number) {
    char tmp_str[100] = {};
    sprintf(tmp_str, "%lg", number);
    set_string(tmp_str);
}

void v_TextField::fit_width_to_string() {
    Vec2d last_char_pos = char_pos(line.len() + 1);
    body.size.content[0] = last_char_pos.x() + 4 * PX_TF_PADDING;
    v_content->get_body().size.content[0] = body.size.content[0] - 2 * PX_TF_PADDING;
}

void v_TextField::copy_to_clipboard() {
    char *str = (char*) calloc(line.len(), sizeof(char));
    int from = line.selection_l();
    int to = line.selection_r();
    strncpy(str, line.c_str() + from, to - from);
    
    sf::Clipboard::setString(str);
    logger.info("text_field", "copied [%s] to the clipboard", str);

    free(str);
}


char to_ascii(uint32_t unicode) {
    return (char) unicode;
}

void convert(char *dst, const uint32_t *unicode, int len) {
    for (int w = 0, r = 0; r < len; ++w, ++r) {
        if (unicode[r] < 128) {
            dst[w] = unicode[r];
        } else {
            dst[w] = ' ';
        }
    }
    dst[len] = '\0';
}

void v_TextField::paste_from_clipboard() {
    int len = (int) sf::Clipboard::getString().getSize();

    char *data = (char*) calloc(len + 1, sizeof(char));
    convert(data, sf::Clipboard::getString().getData(), len);

    line.insert_str(data);
    display();

    logger.info("text_field", "pasted [%s] to the clipboard", data);

    free(data);
}


TextEnterAcceptor::TextEnterAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult TextEnterAcceptor::operator()(const Event::TextEnter &event, const EventAccResult *) {
    if (!acceptor->selected) return EventAccResult::none;
    if (!event.is_symbolic()) return EventAccResult::none;

    char c = event.ascii();

    if (c == '\r' || c == '\n' || c == '\t') {
        // acceptor->add_char('\n'); we don't want add new lines chars in ONE line input
    } else {
        acceptor->add_char(c);
    }
    acceptor->display();

    return EventAccResult::done;
}


KeyDownTextFieldAcceptor::KeyDownTextFieldAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult KeyDownTextFieldAcceptor::operator()(const Event::KeyDown &event, const EventAccResult *) {
    if (!acceptor->selected) return EventAccResult::none;
    
    switch (event.code) {

        case Keyboard::Key::backspace :
            acceptor->line.erase_prev();
            acceptor->display();
            break;

        case Keyboard::Key::del :
            acceptor->line.erase_next();
            acceptor->display();
            break;

        case Keyboard::Key::left :
            if (Keyboard::is_pressed_ctrl()) {
                if (Keyboard::is_pressed_ctrl() == 2) {
                    acceptor->line.shift_selection_l();
                } else {
                    acceptor->line.cursor_word_l();
                }
            } else {
                acceptor->line.cursor_l();
            }
            acceptor->display();
            break;

        case Keyboard::Key::right :
            if (Keyboard::is_pressed_ctrl()) {
                if (Keyboard::is_pressed_ctrl() == 2) {
                    acceptor->line.shift_selection_r();
                } else {
                    acceptor->line.cursor_word_r();
                }
            } else {
                acceptor->line.cursor_r();
            }
            acceptor->display();
            break;

        case Keyboard::Key::lshift :
        case Keyboard::Key::rshift :
            acceptor->line.fix_anchors();
            acceptor->selection_online++;
            break;
        
        #ifdef __APPLE__
        #define CHECK_CTRL_PRESS Keyboard::is_pressed_system()
        #else
        #define CHECK_CTRL_PRESS Keyboard::is_pressed_ctrl()
        #endif
        
        case Keyboard::Key::c :
            if (CHECK_CTRL_PRESS) {
                acceptor->copy_to_clipboard();
            }
            break;
        
        case Keyboard::Key::v :
            if (CHECK_CTRL_PRESS) {
                acceptor->paste_from_clipboard();
            }
            break;
        
        case Keyboard::Key::x :
            if (CHECK_CTRL_PRESS) {
                acceptor->copy_to_clipboard();
                acceptor->line.cut();
                acceptor->display();
            }
            break;
        
        case Keyboard::Key::z :
            if (CHECK_CTRL_PRESS) {
                acceptor->line.undo();
                acceptor->display();
            }
            break;
        
        case Keyboard::Key::y :
            if (CHECK_CTRL_PRESS) {
                acceptor->line.redo();
                acceptor->display();
            }
            break;
        
        case Keyboard::Key::a :
            if (CHECK_CTRL_PRESS) {
                acceptor->line.select_all(acceptor->selection_online);
                acceptor->display();
            }
            break;
        
        case Keyboard::Key::home :
            acceptor->line.cursor_home();
            acceptor->display();
            break;

        case Keyboard::Key::end :
            acceptor->line.cursor_end();
            acceptor->display();
            break;
        
        case Keyboard::Key::enter :
            acceptor->deselect_text_field(true);

        
        default:
            break;
    }

    return EventAccResult::done;
}

const char *v_TextField::get_string() const {
    return line.c_str();
}

KeyUpTextFieldAcceptor::KeyUpTextFieldAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult KeyUpTextFieldAcceptor::operator()(const Event::KeyUp &event, const EventAccResult *) {
    if (!acceptor->selected) return EventAccResult::none;
    
    switch (event.code) {

        case Keyboard::Key::lshift :
        case Keyboard::Key::rshift :
            acceptor->selection_online--;
            if (!acceptor->selection_online) {
                acceptor->line.free_anchors();
            }
            break;
        
        default:
            break;
    }

    return EventAccResult::done;
}

TextFieldMousePressAcceptor::TextFieldMousePressAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult TextFieldMousePressAcceptor::operator()(const Event::MousePress &event, const EventAccResult *) {
    if (!acceptor->redactable) return EventAccResult::none;

    if (acceptor->is_inside(event.position)) {
        acceptor->select();
        acceptor->put_cursor_under_mouse(event.position);
        acceptor->selection_online++;
        acceptor->line.fix_anchors();
        acceptor->pressed = true;
        return EventAccResult::cont;
    } else {
        if (acceptor->is_selected()) {
            acceptor->deselect_text_field();
        }
    }

    return EventAccResult::cont;
}

TextFieldMouseReleaseAcceptor::TextFieldMouseReleaseAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult TextFieldMouseReleaseAcceptor::operator()(const Event::MouseRelease &, const EventAccResult *) {
    if (acceptor->selected) {
        acceptor->selection_online--;
        if (acceptor->selection_online == 0)
            acceptor->line.free_anchors();
        acceptor->pressed = false;
    }

    return EventAccResult::none;
}

TextFieldMouseMoveAcceptor::TextFieldMouseMoveAcceptor(v_TextField *acceptor) :
EventAcceptor(acceptor)
{}

EventAccResult TextFieldMouseMoveAcceptor::operator()(const Event::MouseMove &event, const EventAccResult *) {
    if (!acceptor->selected || !acceptor->is_pressed()) return EventAccResult::none;
    acceptor->put_cursor_under_mouse(event.to);

    return EventAccResult::cont;
}

#undef CHECK_CTRL_PRESS

TextFieldChangeStringSynchronizer::TextFieldChangeStringSynchronizer(char **text_ptr):
text(text_ptr)
{}

EventAccResult TextFieldChangeStringSynchronizer::operator()(const Event::TextChanged &event, const EventAccResult*) {
    if (!text) return EventAccResult::none;

    if (*text) {
        free(*text);
    }
    *text = strdup(event.text);

    return EventAccResult::cont;
}

TextFieldChangeValueSynchronizer::TextFieldChangeValueSynchronizer(double *val_d, int *val_i) :
val_d(val_d),
val_i(val_i)
{}

TextFieldChangeValueSynchronizer::TextFieldChangeValueSynchronizer(int *val_i) :
TextFieldChangeValueSynchronizer(nullptr, val_i)
{}

TextFieldChangeValueSynchronizer::TextFieldChangeValueSynchronizer(double *val_d):
TextFieldChangeValueSynchronizer(val_d, nullptr)
{}

EventAccResult TextFieldChangeValueSynchronizer::operator()(const Event::TextChanged &event, const EventAccResult*) {
    if (val_d) {
        *val_d = event.val_d;
    }

    if (val_i) {
        *val_i = event.val_i;
    }

    return EventAccResult::cont;
}
