#pragma once


#include "line_pos.h"


struct Cursor {
    LinePos pos;
    LinePos anchor;

    bool anchor_fixed = false;

    Cursor(LinePos pos);

    void fix_anchor();
    void free_anchor();

    void set(int idx);

    void move_r();
    void move_l();

    void word_r();
    void word_l();

    void move_home();
    void move_end();

    void select_all(bool to_fix_anchor = false);

    bool anchor_magnet();
    void pull_anchor();

    bool is_space() const { char c = pos.get_c(); return c == ' ' || c == '\n' || c == '\t' || c == '\0'; }

    void shift_selection_r() { pos.move_r(); anchor.move_r(); }
    void shift_selection_l() { pos.move_l(); anchor.move_l(); }

    bool is_selection();

};
