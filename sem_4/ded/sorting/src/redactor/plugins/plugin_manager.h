#pragma once


#include "plugin.h"

#include <vector>
#include <map>


class ToolManager;
class Tool;
class EffectManager;


class PluginManager {
    std::vector<RedactorPlugin*> plugins;
    ToolManager   *tool_manager;
    EffectManager *effect_manager;

public:
    PluginManager(ToolManager *mg_tool, EffectManager *mg_effect);
    ~PluginManager();

    RedactorPlugin *load(const char *filename, RedactorPluginInterface *app_interface);
    void deinit();
    void fit_plugin(RedactorPlugin *plugin);

    std::vector<RedactorPlugin*> &get_plugins() { return plugins; }

    // void set_plugin(const PUPPY::PluginInterface *plugin_self, RedactorPlugin *plugin);
    // RedactorPlugin *get_plugin(const PUPPY::PluginInterface *plugin_self);
};
