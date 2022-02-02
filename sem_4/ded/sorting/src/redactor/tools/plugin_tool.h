#pragma once


#include "tool.h"
#include "redactor/plugins/plugin.h"


class t_Plugin : public Tool {
protected:
    RedactorPlugin *plugin;

public:
    t_Plugin(Tool *manager, RedactorPlugin *plugin);

    virtual void on_mouse_down(const Vec2d &pos) override;
    virtual void on_mouse_up(const Vec2d &pos) override;
    virtual void on_mouse_move(const Vec2d &from, const Vec2d &to) override;

    virtual void on_activate() override;
    virtual void on_update() override;

    virtual void open_settings() override;
};
