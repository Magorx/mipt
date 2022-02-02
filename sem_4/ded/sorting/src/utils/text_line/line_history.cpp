#include "line_history.h"
#include "line.h"


LineHisotryState::LineHisotryState(Line *line, int from, int to) : 
line(line),
str(),
from(from),
to(to)
{
    str.reserve(std::max(0, from - to + 5));
    for (int i = from; i < to; ++i) {
        str.push_back(line->c_str()[i]);
    }
    str.push_back('\0');
}


LineHisotryStateInsertion::LineHisotryStateInsertion(Line *line, int from, int to) :
LineHisotryState(line, from, to)
{}

void LineHisotryStateInsertion::undo() {
    line->free_anchors();
    line->cursor_set(from);
    line->fix_anchors();
    line->cursor_set(to);
    line->erase_prev();
    line->free_anchors();
}

void LineHisotryStateInsertion::redo() {
    line->free_anchors();
    line->cursor_set(from);
    line->insert_str(&str[0]);
}


LineHisotryStateDeletion::LineHisotryStateDeletion(Line *line, int from, int to, bool right) :
LineHisotryState(line, from, to),
right(right)
{}

void LineHisotryStateDeletion::undo() {
    line->free_anchors();
    line->cursor_set(from);
    line->insert_str(&str[0]);

    if (!right) {
        line->cursor_set(from);
    }
}

void LineHisotryStateDeletion::redo() {
    line->free_anchors();
    line->cursor_set(from);
    line->fix_anchors();
    line->cursor_set(to);
    line->erase_prev();
}
