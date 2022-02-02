#pragma once


#include "utils.h"
#include "redactor/layer/layer.h"


class RedactorPlugin;

class PluginEffect : public Effect<Layer> {
protected:
    RedactorPlugin *plugin;

public:
    PluginEffect(RedactorPlugin *plugin);

    const char *get_name() const;

    virtual void apply() override;

    void open_settings();
};
