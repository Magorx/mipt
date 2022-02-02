#pragma once


#include <cstring>

#include "view/view.h"
#include "../renderables/renderables.h"


class v_Text : public AbstractView {
    char *label;
    int char_size;
    RGBA back_color;
    RGBA font_color;

public:
    bool centrized;

    v_Text(const ViewBody &body, const char *label, int char_size, RGBA font_color, RGBA back_color = {0, 0, 0, 0}, bool centrized = false, AbstractView *parent = nullptr);
    virtual ~v_Text();

    virtual void render(Renderer *renderer) override;

    char *get_label() { return label; }
};
