#pragma once


#include "redactor/plugin_std/plugin_std.hpp"
#include "redactor/plugins/plugin.h"

#include "redactor/plugins/interface/target.h"
#include "redactor/plugins/interface/widget_factory.h"
#include "redactor/plugins/interface/shader_factory.h"
#include "redactor/plugins/interface/target_factory.h"


class AbstractView;

class RedactorPluginInterface : public PUPPY::AppInterface {
    const RedactorPlugin *plugin;

    PUPPY::Widget *root_widget;
    AbstractView *root;

public:
    RedactorPluginInterface();
    ~RedactorPluginInterface();

    void set_plugin(const RedactorPlugin *plugin_);

// extension
    virtual bool  ext_enable        (const char *name)                        const override;
    virtual void *ext_get_func      (const char *extension, const char *func) const override;
    virtual void *ext_get_interface (const char *extension, const char *name) const override;
    virtual void  ext_register_as   (const char *extension)                   const override;

// general
    virtual void log(const char *fmt, ...) const override;
    virtual double get_absolute_time()     const override;

    virtual PUPPY::RGBA get_color() const override;
    virtual float get_size()    const override;

    virtual const std::vector<PUPPY::WBody> get_windows() const override;
    virtual PUPPY::Widget *get_root_widget() const override;

// target
    virtual PUPPY::RenderTarget *get_target()  const override;
    virtual PUPPY::RenderTarget *get_preview() const override;
    virtual void flush_preview()           const override;

    virtual void set_color(const PUPPY::RGBA &color) const override;
    virtual void set_size(float size) const override;
};
