#include "plugin_manager.h"
#include "utils/logger.h"

#include "redactor/engine.h"

#include "redactor/tools/plugin_tool.h"


PluginManager::PluginManager(ToolManager *mg_tool, EffectManager *mg_effect) : 
tool_manager(mg_tool),
effect_manager(mg_effect)
{}

PluginManager::~PluginManager() {
    for (auto plugin : plugins) {
        delete plugin;
    }
}

void PluginManager::deinit() {
    for (auto plugin : plugins) {
        plugin->deinit();
    }
}

RedactorPlugin *PluginManager::load(const char *filename, RedactorPluginInterface *app_interface) {
    logger.info("PluginManager", "loading plugin, filename[%s]\n", filename);

    if (!filename || !app_interface) {
        logger.error("PluginManager", "bad params for plugin load: filename[%s], app_interface[%p]", filename, app_interface);
    }

    RedactorPlugin *plugin = new RedactorPlugin(filename, app_interface, this);
    if (!plugin || !plugin->is_ok()) {
        logger.error("PluginManager", "plugin[%p] with filename[%s] was technicaly loaded,", plugin, filename);
        logger.error("PluginManager", "but somewhy is not ok, deleting it");
        delete plugin;
        return nullptr;
    }

    plugins.push_back(plugin);

    fit_plugin(plugin);

    return plugin;
}

void PluginManager::fit_plugin(RedactorPlugin *plugin) {
    if (!plugin) return;

    if (plugin->get_type() == PUPPY::PluginType::TOOL) {
        tool_manager->add_tool(new t_Plugin(tool_manager, plugin));
    } else if (plugin->get_type() == PUPPY::PluginType::EFFECT) {
        effect_manager->add(new PluginEffect(plugin));
    }
}

// void PluginManager::set_plugin(const PPluginInterface *plugin_self, RedactorPlugin *plugin) {
//     plugin_map[plugin_self] = plugin;
// }

// RedactorPlugin *PluginManager::get_plugin(const PPluginInterface *plugin_self) {
//     if (!plugin_map.count(plugin_self)) {
//         logger.error("plugin_manager", "plugin with interface [%p] is not registered in manager [%p], returning nullptr for request", plugin_self, this);
//         return nullptr;
//     } else {
//         return plugin_map[plugin_self];
//     }
// }
