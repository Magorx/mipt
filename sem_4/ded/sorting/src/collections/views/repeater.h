#pragma once


#include "highlighter.h"


class v_Repeater : public v_Highlighter {
    double stub_size;
    // int coord;

public:
    v_Repeater(const ViewBody &body, Appearence *appr, double stub_size);

    virtual void render(Renderer *renderer) override;
};
