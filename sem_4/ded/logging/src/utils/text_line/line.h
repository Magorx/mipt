#pragma once

#include "utils/logger.h"
#include <vector>
#include <utility>

#include "line_pos.h"
#include "cursor.h"

#include "line_history.h"

/*
[some_sting ]
           ^ cursor.pos()
*/

class Line {
    std::vector<char> data;
    Cursor cursor;

    History history;
    LineHisotryStateInsertion *cur_ins_state;
    LineHisotryStateDeletion  *cur_del_state;

    void flush_history_states();

public:
    Line(size_t to_reserve = 10);

    int len() const;
    
    void insert_char(char c);
    void put_char(char c);

    void insert_str(const char *str);
    void set_str(const char *str);

    void erase_next();
    void erase_prev();

    void cut(bool include_border = false);

    void cursor_set(int idx) { cursor.set(idx); }
    void cursor_l() { cursor.move_l(); }
    void cursor_r() { cursor.move_r(); }
    void cursor_home() { cursor.move_home(); }
    void cursor_end()  { cursor.move_end();  }
    void select_all(bool to_fix_anchor) { cursor.select_all(to_fix_anchor); }

    void cursor_word_r() { cursor.word_r(); }
    void cursor_word_l() { cursor.word_l(); }

    bool cursor_mag() { return cursor.anchor_magnet(); }

    int selection_l() { return std::min(cursor.pos(), cursor.anchor()); }
    int selection_r() { return std::max(cursor.pos(), cursor.anchor()); }

    int cursor_pos() { return cursor.pos(); }
    int anchor_pos() { return cursor.anchor(); }

    char get_c(size_t idx) const { if (idx >= data.size()) return '\0'; else return data[idx]; }

    void fix_anchors();
    void free_anchors();

    void shift_selection_r() { cursor.shift_selection_r(); }
    void shift_selection_l() { cursor.shift_selection_l(); }

    void undo();
    void redo();

    inline const char *c_str() const { return &data[0]; }
    inline char *c_str() { return &data[0]; }

    void dump(char endline = '\n') const;
};
