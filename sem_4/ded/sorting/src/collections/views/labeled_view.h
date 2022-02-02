#pragma once


#include "hideable.h"
#include "text.h"


class AbstractLabledView : public v_Hideable {
protected:
    v_Text *v_label;
public:
    AbstractLabledView(const ViewBody &body, AbstractView *parent = nullptr, bool to_block_covered = false, bool to_pass_inactive = false, bool is_shown = true);

    virtual ~AbstractLabledView();

    virtual void render(Renderer *renderer) override;

    void add_label(const char *label, int char_size, RGBA font_color, RGBA back_color = {0, 0, 0, 0}, bool centrized = true);

    char *get_label_text();

    virtual void refit() override;
};
