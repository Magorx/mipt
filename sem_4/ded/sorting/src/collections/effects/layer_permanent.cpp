#include "layer_permanent.h"
#include "redactor/plugins/plugin.h"

#include "redactor/engine.h"

PluginEffect::PluginEffect(RedactorPlugin *plugin) :
Effect(nullptr),
plugin(plugin)
{}

const char *PluginEffect::get_name() const {
    if (!plugin) return "NOPLUGIN"; 
    else return plugin->get_lib()->name;
}

void PluginEffect::apply() {
    plugin->get_inteface()->effect_apply();

    auto tm = App.app_engine->get_tool_manager(); if (!tm) return;
    auto canvas = tm->get_active_canvas(); if(!canvas) return;

    canvas->flush_draw_to_active();

    canvas->push_history();
}

void PluginEffect::open_settings() {
    // if (!plugin) return;
    // auto settings = plugin->get_settings();
    // if (!settings) return;

    // settings->e_toggle_activity.emit({});
}
