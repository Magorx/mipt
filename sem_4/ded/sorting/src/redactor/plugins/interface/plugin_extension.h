#pragma once


#include "redactor/extensions/extension.h"
#include "redactor/plugins/plugin.h"


struct PlugingExtension : public RedactorExtension {
    const RedactorPlugin *plugin;
    
    PlugingExtension(const char *name, uint32_t version, const RedactorPlugin *plugin);

    virtual void *get_func(const char *func) override;

    virtual void *get_interface(const char *name) override;
};
