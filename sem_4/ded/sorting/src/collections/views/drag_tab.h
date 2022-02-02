#pragma once


#include "highlighter.h"


class v_DragPad : public v_Highlighter {
public:
    v_DragPad(const ViewBody &body, SmartColor *color = nullptr, AbstractView *parent = nullptr, double highlight_coef = 0):
    v_Highlighter(body, color, parent, highlight_coef)
    {
        e_mouse_move.add(new AVDragEmitter(this), false);
    }
};
