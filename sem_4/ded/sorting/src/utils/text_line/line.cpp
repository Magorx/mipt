#include "line.h"


Line::Line(size_t to_reserve) :
data(),
cursor(LinePos(this, 0)),
history(50),
cur_ins_state(nullptr),
cur_del_state(nullptr)
{
    data.resize(to_reserve, 0);
    data.resize(1);
    data[0] = '\0';
    data.reserve(to_reserve);
}

int Line::len() const { return (int) data.size(); }

void Line::cut(bool include_border) {
    int from = cursor.pos();
    int to   = cursor.anchor();

    bool right = false;
    if (from > to) {
        right ^= true;
        std::swap(from, to);
    }

    if (from == to + include_border) return;
    history.add(new LineHisotryStateDeletion(this, from, to + include_border, right));

    for (int w = from, r = to + include_border; r < len(); ++w, ++r) {
        data[w] = data[r];
    }
    data.resize(data.size() - (to - from + include_border));

    cursor.anchor_magnet();
}

void Line::insert_char(char c) {
    cut();
    data.insert(data.begin() + cursor.pos(), c);

    history.add(new LineHisotryStateInsertion(this, cursor.pos(), cursor.pos() + 1));

    if (cursor.pos() == len() - 1) {
        data.push_back('\0');
    }

    cursor.move_r();
}

void Line::put_char(char c) {
    cursor.move_l();
    cut(true);
    data.insert(data.begin() + cursor.pos(), c);
}

void Line::insert_str(const char *str) {
    if (cursor.is_selection()) {
        cut();
    }

    ScopeHistoryAdditionBlocker add_blocker(history);

    const int prev_cursor_pos = cursor.pos();
    int l = strlen(str);
    for (int i = 0; i < l; ++i) {
        insert_char(str[i]);
    }
    const int cur_cursor_pos = cursor.pos();

    add_blocker.restore();

    history.add(new LineHisotryStateInsertion(this, prev_cursor_pos, cur_cursor_pos));
}

void Line::set_str(const char *str) {
    int l = strlen(str);
    data.resize(l);
    for (int i = 0; i < l; ++i) {
        data[i] = str[i];
    }
    data.push_back('\0');
    cursor_end();
}

void Line::erase_next() {
    if (cursor.is_selection()) {
        cut();
    } else {
        if (cursor.pos() == len() - 1) return;
        cut(true);
    }
}

void Line::erase_prev() {
    if (cursor.is_selection()) {
        cut();
    } else {
        if (!cursor.pos()) return;

        cursor.move_l();
        erase_next();
    }
}

void Line::fix_anchors() {
    cursor.fix_anchor();
}

void Line::free_anchors() {
    cursor.free_anchor();
}

void Line::dump(char endline) const {
    int pos = cursor.pos();
    int anc = cursor.anchor();

    if (anc == pos) {
        printf("[");
        for (int i = 0; i < pos; ++i) {
            printf("%c", data[i]);
        }
        printf("|");
        for (int i = pos; i < len(); ++i) {
            printf("%c", data[i]);
        }
        printf("]%c", endline);
    } else {
        int mn = std::min(pos, anc);
        int mx = std::max(pos, anc);

        printf("[");
        for (int i = 0; i < mn; ++i) {
            printf("%c", data[i]);
        }
        printf("%c", mn == anc ? '{' : '|');
        for (int i = mn; i < mx; ++i) {
            printf("%c", data[i]);
        }
        printf("%c", mn == anc ? '|' : '}');
        for (int i = mx; i < len(); ++i) {
            printf("%c", data[i]);
        }
        printf("]%c", endline);
    }
}

void Line::flush_history_states() {
    if (cur_ins_state) {
        history.add(cur_ins_state);
        cur_ins_state = nullptr;
    }

    if (cur_del_state) {
        history.add(cur_ins_state);
        cur_ins_state = nullptr;
    }
}

void Line::undo() {
    flush_history_states();
    history.undo();
}

void Line::redo() {
    flush_history_states();
    history.redo();
}
