#include <dlfcn.h>

#include "plugin.h"
#include "utils/logger.h"

#include "redactor/plugins/interface/app_plugin_interface.h"
#include "plugin_manager.h"


RedactorPlugin::RedactorPlugin(const char *fileName, RedactorPluginInterface *appInterface, PluginManager *) :
lib_handle(nullptr),

status(-1),

lib(nullptr),
interface(nullptr)

{
    std::filesystem::path path = "./" + std::string(fileName);
    path = path.parent_path();

    lib_handle = dlopen(fileName, RTLD_NOW);
    if (lib_handle == nullptr) {
        logger.error("Plugin", "can't dlopen plugin named [%s]: %s", fileName, dlerror());
        return;
    }

    PUPPY::PluginGetInterfaceType plugin_init = (PUPPY::PluginGetInterfaceType) dlsym(lib_handle, PUPPY::GET_INTERFACE_FUNC);
    if (plugin_init == nullptr) {
        logger.error("Plugin", "can't get interface of plugin [%s]", fileName);
        dlclose(lib_handle);
        return;
    }
    appInterface->set_plugin(this);
    
    interface = plugin_init();
    if (!interface) {
        logger.error("Plugin", "plugin [%s] returned nullptr interface", fileName);
        dlclose(lib_handle);
        return;
    }

    lib = interface->get_info();
    if (!lib) {
        logger.error("Plugin", "plugin [%s] returned nullptr information", fileName);
        dlclose(lib_handle);
        return;
    }

    PUPPY::Status init_status = interface->init(appInterface, path);
    if (init_status != PUPPY::OK) {
        logger.error("Plugin", "initialization of plugin [%s] failed", fileName);
        dlclose(lib_handle);
        return;
    }

    logger.info("Plugin", "loaded plugin [%s] properly. Author: %s, version: %s (std %lu).\n", lib->name, lib->version, lib->std_version);

    status = 0;
}

RedactorPlugin::~RedactorPlugin() {
    if (status >= 0) {
        // interface->deinit();
        status = -1;
    }

    if (!lib_handle) {
        logger.error("~Plugin", "a plugin doesn't even have dl_handle");
    } else {
        if (dlclose(lib_handle) != 0) {
            logger.error("~Plugin", "can't close plugin: %s", dlerror());
        }
    }
}

void RedactorPlugin::on_mouse_press(const Vec2d &position) {
    interface->tool_on_press(to_pvec2d(position));
}

void RedactorPlugin::on_mouse_move(const Vec2d &from, const Vec2d &to) {
    interface->tool_on_move(to_pvec2d(from), to_pvec2d(to));
}

void RedactorPlugin::on_mouse_release(const Vec2d &position) {
    interface->tool_on_release(to_pvec2d(position));
}

void RedactorPlugin::apply() {
    interface->effect_apply();
}
