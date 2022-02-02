#include "cursor.h"


Cursor::Cursor(LinePos pos) :
pos(pos),
anchor(pos)
{}

void Cursor::fix_anchor()  { anchor_fixed = true; }
void Cursor::free_anchor() { anchor_fixed = false; }

void Cursor::pull_anchor() {
    if (!anchor_fixed) {
        anchor = pos;
    }
}

void Cursor::set(int idx) {
    pos.set(idx);
    pull_anchor();
}

void Cursor::move_r() {
    if (is_selection() && !anchor_fixed) {
        if (anchor() > pos()) {
            pos = anchor;
        } else {
            anchor = pos;
        }
        return;
    }

    pos.move_r();
    pull_anchor();
}

void Cursor::move_l() {
    if (is_selection() && !anchor_fixed) {
        if (anchor() < pos()) {
            pos = anchor;
        } else {
            anchor = pos;
        }
        return;
    }

    pos.move_l();
    pull_anchor();
}

//  let oldIsWhitespace = this.isWhitespace(this.value[this.cursorPosition])

//         while(this.cursorPosition > 0) {
//             this.cursorPosition--;
//             let currentIsWhitespace = this.isWhitespace(this.value[this.cursorPosition])

//             if(currentIsWhitespace && !oldIsWhitespace) {
//                 break;
//             }

//             oldIsWhitespace = this.isWhitespace(this.value[this.cursorPosition])
//         }

//         this.cursorMoved()

void Cursor::word_l() {
    if (pos.is_home()) return;

    pull_anchor();

    move_l();

    bool prev_whitespace = is_space();

    while (!pos.is_home()) {
        pos.move_l();
        bool cur_whitespce = is_space();

        if (cur_whitespce && !prev_whitespace) { pos.move_r(); break; }

        prev_whitespace = cur_whitespce;
    }

    pull_anchor();
}


void Cursor::word_r() {
    if (pos.is_end()) return;

    pull_anchor();

    bool prev_whitespace = is_space();

    while (!pos.is_end()) {
        pos.move_r();
        bool cur_whitespce = is_space();

        if (cur_whitespce && !prev_whitespace) break;

        prev_whitespace = cur_whitespce;
    }

    pull_anchor();
}
    
void Cursor::move_home() {
    pos.move_home();
    pull_anchor();
}

void Cursor::move_end() {
    pos.move_end();
    pull_anchor();
}


void Cursor::select_all(bool to_fix_anchor) {
    anchor.move_home();
    pos.move_home();
    fix_anchor();
    pos.move_end();
    if (!to_fix_anchor) {
        free_anchor();
    }
}

bool Cursor::anchor_magnet() {
    if (pos() > anchor()) {
        pos = anchor;
        return false;
    } else {
        anchor = pos;
        return true;
    }
}

bool Cursor::is_selection() {
    return pos() != anchor();
}
