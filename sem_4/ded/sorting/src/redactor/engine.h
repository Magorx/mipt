#pragma once


#include "MogaEngine.h"

#include "tools/tool_manager.h"
#include "tools/tools.h"

#include "effect_manager.h"

#include "redactor/plugins/interface/app_plugin_interface.h"
#include "plugins/plugin_manager.h"
#include "redactor/extensions/extension_manager.h"

#include <vector>


class RedactorEngine : public MogaEngine {
    ToolManager   *tool_manager;
    EffectManager *effect_manager;
    PluginManager *plugin_manager;
    ExtensionManager *extension_manager;

    std::vector<v_Window*> windows;

public:
    RedactorEngine(RWindow *window,
                   const char *name);
    
    ~RedactorEngine();
    
    inline ToolManager      *get_tool_manager      () { return tool_manager;      }
    inline EffectManager    *get_effect_manager    () { return effect_manager;    }
    inline PluginManager    *get_plugin_manager    () { return plugin_manager;    }
    inline ExtensionManager *get_extension_manager () { return extension_manager; }

    inline RedactorPluginInterface *get_plugin_interface();

    virtual void frame_init_tick() override;

    bool load_plugin(const char *path);
    bool load_plugin(const char *path, bool directory);

    void update_windows();
    std::vector<v_Window*> &get_windows() {
        return windows;
    }
};
