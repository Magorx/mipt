#pragma once


#include "abstract_layout.h"


class v_HorizontalLayout : public AbstractLayoutView {
public:
    v_HorizontalLayout(ViewBody body, ViewBody padding_body = {{0, 0}, {0, 0}}, double padding_split = 0, AbstractView *parent = nullptr, SmartColor *color = nullptr, double highlight_coef = 0);
    virtual ~v_HorizontalLayout();

    virtual void layout_refit() override;
};
