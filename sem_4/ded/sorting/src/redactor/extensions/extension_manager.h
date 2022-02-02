#pragma once


#include "extension.h"
#include "redactor/plugins/interface/plugin_extension.h"

#include <map>
#include <string_view>


class ExtensionManager {
    std::map<const std::string_view, RedactorExtension*> extensions;
public:
    ExtensionManager();
    ~ExtensionManager();

    RedactorExtension *get_extension(const std::string_view &name);
    void register_extension(const std::string_view &name, const RedactorPlugin *plugin);
};
