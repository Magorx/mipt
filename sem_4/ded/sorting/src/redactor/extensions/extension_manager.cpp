#include "extension_manager.h"
#include "utils/logger.h"


ExtensionManager::ExtensionManager() :
extensions()
{

}

ExtensionManager::~ExtensionManager() {
    for (auto &ext : extensions) {
        delete ext.second;
    }
}

RedactorExtension *ExtensionManager::get_extension(const std::string_view &name) {
    auto ext = extensions.find(name);
    if (ext == extensions.end()) {
        logger.warning("ExtensionManager", "request for extension [%s] failed", name.begin());
        return nullptr;
    }

    return (*ext).second;
}

void ExtensionManager::register_extension(const std::string_view &name, const RedactorPlugin *plugin) {
    extensions[name] = new PlugingExtension(name.begin(), plugin->interface->std_version, plugin);
}
