#include "plugin_extension.h"
#include "utils/logger.h"


PlugingExtension::PlugingExtension(const char *name, uint32_t version, const RedactorPlugin *plugin) :
RedactorExtension(name, version),
plugin(plugin)
{}

void *PlugingExtension::get_func(const char *func) {
    if (!func || !plugin) return nullptr;

    auto ret = plugin->interface->ext_get_func(name, func);
    if (!ret) {
        logger.warning("PlugingExtension", "request for function [%s] from extension [%s] failed", func, this->name);
    }

    return ret;
}

void *PlugingExtension::get_interface(const char *name) {
    if (!name || !plugin) return nullptr;

    auto ret = plugin->interface->ext_get_interface(this->name, name);
    if (!ret) {
        logger.warning("PlugingExtension", "request for interface [%s] from extension [%s] failed", name, this->name);
    }

    return ret;
}
