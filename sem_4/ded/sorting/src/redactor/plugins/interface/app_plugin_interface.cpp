#include "app_plugin_interface.h"
#include "redactor/engine.h"

#include "redactor/plugins/interface/widgets/pwidget.h"


RedactorPluginInterface::RedactorPluginInterface() :
plugin(nullptr),
root_widget(nullptr)
{
    factory.target = new TargetFactory;
    factory.shader = new ShaderFactory;
    factory.widget = new WidgetFactory;

    feature_level = PUPPY::SHADER_SUPPORT | PUPPY::SETTINGS_SUPPORT;

    root = App.engine->main_view;
    auto cur_root_widget = new PluginWidget(root->get_body());
    cur_root_widget->set_view(dynamic_cast<v_Highlighter*>(root));

    root_widget = cur_root_widget;
}

RedactorPluginInterface::~RedactorPluginInterface() {
    delete root_widget;
}

// extension
bool  RedactorPluginInterface::ext_enable(const char *name) const {
    auto manager = App.app_engine->get_extension_manager(); if (!manager) return false;

    if (manager->get_extension(name)) {
        return true;
    } else {
        return false;
    }
}

void *RedactorPluginInterface::ext_get_func(const char *extension, const char *func) const {
    auto manager = App.app_engine->get_extension_manager(); if (!manager) return nullptr;

    auto ext = manager->get_extension(extension); if (!ext) return nullptr;
    void *function = ext->get_func(func);

    return function;
}

void *RedactorPluginInterface::ext_get_interface(const char *extension, const char *name) const {
    auto manager = App.app_engine->get_extension_manager(); if (!manager) return nullptr;

    auto ext = manager->get_extension(extension); if (!ext) return nullptr;
    void *function = ext->get_interface(name);

    return function;
}

void RedactorPluginInterface::ext_register_as(const char *extension) const {
    auto manager = App.app_engine->get_extension_manager(); if (!manager) return;

    manager->register_extension(extension, plugin);
}


// general
void RedactorPluginInterface::log(const char *fmt, ...) const {
    const char *plugin_name = plugin ? plugin->lib->name : "a_plugin";

    va_list args;
    va_start(args, fmt);
    logger.logv((int) Logger::Level::warning, "plgn", plugin_name, fmt, args);
    va_end(args);
}

double RedactorPluginInterface::get_absolute_time() const {
    return App.engine->get_cur_time();
}


PUPPY::RGBA RedactorPluginInterface::get_color() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return {0, 0, 0, 0};

    return tool_manager->get_draw_color().ui32;
}

float RedactorPluginInterface::get_size() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return 0;

    return tool_manager->get_size();
}

const std::vector<PUPPY::WBody> RedactorPluginInterface::get_windows() const {
    App.app_engine->update_windows();
    auto cur_windows = App.app_engine->get_windows();

    std::vector<PUPPY::WBody> ret;
    for (auto window : cur_windows) {
        auto window_body = window->get_body();
        window_body.position += window->get_header()->get_body().position;
        ret.push_back(to_wbody(window_body));
    }

    return ret;
}

PUPPY::Widget *RedactorPluginInterface::get_root_widget() const {
    return root_widget;
}



// target
PUPPY::RenderTarget *RedactorPluginInterface::get_target() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return nullptr;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return nullptr;

    auto layer = canvas->get_active_layer();
    if (!layer) return nullptr;

    return new Target(layer, false, true);
}

PUPPY::RenderTarget *RedactorPluginInterface::get_preview() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return nullptr;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return nullptr;

    auto layer = canvas->get_draw_layer();
    if (!layer) return nullptr;

    return new Target(layer);
}

void RedactorPluginInterface::flush_preview() const {
    auto tool_manager = App.app_engine->get_tool_manager();
    if (!tool_manager) return;

    auto canvas = tool_manager->get_active_canvas();
    if (!canvas) return;

    canvas->flush_draw_to_active();
}

void RedactorPluginInterface::set_color(const PUPPY::RGBA &color) const {
    auto tm = App.app_engine->get_tool_manager(); if (!tm) return;

    tm->set_draw_color(color.ui32);
}

void RedactorPluginInterface::set_size(float size) const {
    auto tm = App.app_engine->get_tool_manager(); if (!tm) return;

    tm->set_size(size);
}

void RedactorPluginInterface::set_plugin(const RedactorPlugin *plugin_) {
    plugin = plugin_;
}