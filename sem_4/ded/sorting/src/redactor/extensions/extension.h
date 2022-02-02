#pragma once

#include <cstdint>

struct RedactorExtension {
    const char *name;
    uint32_t version;

    RedactorExtension(const char *name, uint32_t version) :
    name(name),
    version(version)
    {}

    virtual ~RedactorExtension() {}

    virtual void *get_func(const char *func) = 0;
    virtual void *get_interface(const char *name) = 0;
};
