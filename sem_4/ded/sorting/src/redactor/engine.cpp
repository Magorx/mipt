#include "engine.h"
#include "hotkey_bind.h"

#include "overall_redactor_reactions.h"

#include "redactor/plugins/interface/app_plugin_interface.h"

#include <string>
#include <filesystem>

RedactorEngine::RedactorEngine(RWindow *window,
                               const char *name) :
MogaEngine(window, name),
tool_manager(new ToolManager(visual->get_renderer())),
effect_manager(new EffectManager),
plugin_manager(new PluginManager(tool_manager, effect_manager)),
extension_manager(new ExtensionManager)
{
    t_Brush *t_brush         = new t_Brush(tool_manager);
    t_Eraser *t_eraser       = new t_Eraser(tool_manager);
    t_ColorPipett *t_pipette = new t_ColorPipett(tool_manager);

    tool_manager->add_tool(t_brush,   HotkeyBind::brush);
    tool_manager->add_tool(t_eraser,  HotkeyBind::eraser);
    tool_manager->add_tool(t_pipette, HotkeyBind::pipette);

    tool_manager->set_active_tool(HotkeyBind::brush);
    tool_manager->set_size(13);

    auto alt_key_tool_restorer = new ToolManagerAltToolOff(tool_manager);
    main_view->e_key_up.add(alt_key_tool_restorer, false);
    main_view->e_key_down.add(new ToolManagerAltToolOn(tool_manager, alt_key_tool_restorer), false);

    main_view->e_key_down.add(new ToolManagerHotkeys(tool_manager), false);
    main_view->e_scroll.add(new ToolManagerScrollShiftToolSize(tool_manager), false);
}

RedactorEngine::~RedactorEngine() {
    delete tool_manager;
    delete effect_manager;
    delete extension_manager;

    plugin_manager->deinit();

    delete main_view;
    main_view = nullptr;

    delete plugin_manager;
}

void RedactorEngine::frame_init_tick() {
    MogaEngine::frame_init_tick();

    for (auto &plugin : plugin_manager->get_plugins()) {
        plugin->on_tick(dt);
    }
}

bool RedactorEngine::load_plugin(const char *path) {
    auto interface = get_plugin_interface();
    if (!interface) {
        logger.ERROR("RedactorEngine", "could't create plugin interface somewhy");
        return false;
    }

    if (auto plugin = plugin_manager->load(path, interface)) {
        interface->set_plugin(plugin);
        return true;
    } else {
        delete interface;
        return false;
    }
}

bool RedactorEngine::load_plugin(const char *path, bool is_dir) {
    if (!is_dir) return load_plugin(path);

    try {
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension().string() == ".so") {
                load_plugin(entry.path().string().c_str());
            }
        }
    } catch (const std::filesystem::filesystem_error &err) {
        logger.warning("load_plugin", "fs error: %s", err.what());
    }

    return true;
}

inline RedactorPluginInterface *RedactorEngine::get_plugin_interface() { return new RedactorPluginInterface; }

void RedactorEngine::update_windows() {
    windows.clear();

    auto subviews = main_view->get_subviews();
    for (auto view : subviews) {
        if (auto w = dynamic_cast<v_Window*>(view)) {
            if (!w->is_active()) continue;

            windows.push_back(w);
        }
    }
}