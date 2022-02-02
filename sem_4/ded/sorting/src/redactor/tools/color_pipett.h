#pragma once


#include "tool.h"


class t_ColorPipett : public Tool {
    public:
    t_ColorPipett(Tool *manager);

    virtual void on_mouse_down(const Vec2d &pos) override;
    virtual void on_mouse_up(const Vec2d &pos) override;
    virtual void on_mouse_move(const Vec2d &from, const Vec2d &to) override;

    virtual void on_activate() override;
    virtual void on_update() override;
};
