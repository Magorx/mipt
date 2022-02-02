#pragma once


#include "redactor/plugin_std/plugin_std.hpp"
#include "utils/vec2d.h"
#include "visual/color/rgba.h"


class RedactorPluginInterface;
class PluginManager;

class RedactorPlugin {
    void *lib_handle;

    int status;

public:
    const PUPPY::PluginInfo      *lib;
    PUPPY::PluginInterface *interface;

    RedactorPlugin(const char *fileName, RedactorPluginInterface *appInterface, PluginManager *manager = nullptr);
    ~RedactorPlugin();

    void on_tick(double dt) { interface->on_tick(dt); }

    void dump() { interface->dump(); }

    void deinit() { interface->deinit(); }

    void on_mouse_press(const Vec2d &position);
    void on_mouse_move(const Vec2d &from, const Vec2d &to);
    void on_mouse_release(const Vec2d &position);

    void apply();

    bool is_ok() {
        return !status;
    }

    const PUPPY::PluginInterface *get_inteface() const { return interface; }
    const PUPPY::PluginInfo *get_lib() const { return lib; }
    PUPPY::PluginType get_type() const { return lib->type; }

    static PUPPY::Vec2f  to_pvec2d(const Vec2d  &vec)   { return PUPPY::Vec2f(vec.x(), vec.y()); }
    static Vec2d   from_pvec2d(const PUPPY::Vec2f &vec) { return    Vec2d(vec.x  , vec.y  ); }

    static PUPPY::RGBA  to_pcolor(const RGBA  &col)   { return col.ui32; }
    static RGBA   from_pcolor(const PUPPY::RGBA &col) { return col.ui32; }

    void show_settings() {
        interface->show_settings();
    }
        
};
